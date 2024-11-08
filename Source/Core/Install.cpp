#include <iostream>
#include <fstream>
#include <vector>
#include <Core/Config.h>
#include <Core/Install.h>
#include <filesystem>
#include <chrono>
#include <curl/curl.h>
#include <array>

std::string packageName;

namespace fs = std::filesystem;
std::string Repo, InstallDIR, PackagesDIR, CacheDIR;

struct ProgressData {
    long totalSize;
    std::ofstream* fileStream;
};

void GetConfig() {
  InitConfig();
  Repo = config->ResolveConfig("Global", "Repo");
  InstallDIR = config->ResolveConfig("Global", "InstallDIR");
  PackagesDIR = config->ResolveConfig("Global", "PackagesDIR");
  CacheDIR = config->ResolveConfig("Global", "CacheDIR");
  CleanConfig();
  if (!std::filesystem::exists(InstallDIR)) {
    if (!std::filesystem::create_directory(InstallDIR)) {
      std::cout << "Error: Installation directory does not exist and could not create it" << std::endl;
      exit(1);
    }
  }
}

void GetPackage(std::basic_string<char> const Package) {
  if (!(Repo.rfind("http://", 0) == 0 || Repo.rfind("https://", 0) == 0)) {
    Repo = "http://" + Repo;
  }
  std::string cacheFilePath = CacheDIR + "/" + Package + ".npe";
  if (std::filesystem::exists(cacheFilePath)) {
    std::cout << "Info: using already cached package for " << Package << std::endl;
    return;
  }
  std::string PackageURL = Repo + "/" + Package + ".npe";
  std::string SignatureURL = Repo + "/" + Package + ".sig";
  packageName = Package + ".sig";
  DownloadPackages(SignatureURL, CacheDIR + "/" + Package + ".sig");
  packageName = Package + ".npe";
  DownloadPackages(PackageURL, CacheDIR + "/" + Package + ".npe");
}

void QuikResolve(int ArgLen, char *Arg[]) {
  GetConfig();
  std::vector<std::string> Packages;
  for(size_t i = 2; i < ArgLen; i++) {
    Packages.push_back(Arg[i]);
  }
  ArgLen = Packages.size();
  auto StartTime = std::chrono::high_resolution_clock::now();
  for(const auto& Package : Packages) {
    std::string FirstTwo = Package.substr(0, 2);
    fs::path PackagePath = fs::path(PackagesDIR) / FirstTwo / Package;
    if (!fs::is_directory(PackagePath)) {
      std::cout << "Error: package " << Package << " not found" << std::endl;
      exit(1);
    }
  }
  auto EndTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> Duration = EndTime - StartTime;
  std::cout << "Resolved package(s) in " << Duration.count() << " seconds" << std::endl;
  InstallPackages(Packages);
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, ProgressData* progressData) {
    size_t totalSize = size * nmemb;
    progressData->fileStream->write(static_cast<const char*>(contents), totalSize);
    return totalSize;
}

void ShowProgressBar(double progress) {
  const int barWidth = 70;
  std::cout << "\r" << packageName << " [";
  int pos = barWidth * progress;
  for(int i = 0; i < barWidth; ++i) {
    if(i == pos) std::cout << ">";
    if(i < pos) std::cout << "=";
    else std::cout << " ";
  }
  std::cout << "]" << int(progress * 100) << " %";
  std::cout.flush();
}

static int ProgressCallback(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded) {
    if (TotalToDownload > 0) {
        double progress = NowDownloaded / TotalToDownload;
        ShowProgressBar(progress);
    }
    return 0;
}

void DownloadPackages(const std::string& URL, const std::string& Path) {
    CURL* curl;
    CURLcode res;
    std::ofstream file(Path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << Path << std::endl;
        exit(1);
    }

    ProgressData progressData;
    progressData.fileStream = &file;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &progressData);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, ProgressCallback);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &progressData);
        
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curl, CURLOPT_STDERR, fopen("/dev/null", "w"));

        res = curl_easy_perform(curl);
        
        if (res != CURLE_OK) {
            std::cerr << "Error: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            file.close();
            exit(1);
        }

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code != 200) {
            std::cerr << "Error: HTTP response code " << http_code << " for " << URL << std::endl;
            curl_easy_cleanup(curl);
            file.close();
            exit(1);
        }
        curl_easy_cleanup(curl);
        file.close();
        std::cout << std::endl;
    }
}

void InstallPackage(std::basic_string<char> const Package) {
    std::string Command = "lz4 -dc < " + CacheDIR + "/" + Package + ".npe | tar -xf - -C " + InstallDIR;
    std::array<char, 128> buffer;
    std::string Result;
    FILE* pipe = popen(Command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error: couldn't extract package '" << Package << "': failed to open pipe." << std::endl;
        exit(1);
    }
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        Result += buffer.data();
    }
    int exitCode = pclose(pipe);
    if (exitCode != 0) {
        std::cerr << "Error: couldn't extract package '" << Package << "': " << Result << std::endl;
        exit(1);
    }
}

void InstallPackages(std::vector<std::string> Packages) {
  for(const auto& Package : Packages) {
    GetPackage(Package);
    InstallPackage(Package);
  }
}
