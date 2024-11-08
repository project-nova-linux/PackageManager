#include <string>
#include <vector>

extern std::string packageName;

void GetConfig();
void GetPackage(std::basic_string<char> const Package);
void InstallPackages(std::vector<std::string> Packages);
void QuikResolve(int ArgLen, char *Arg[]);
struct ProgressData;
void ShowProgressBar(double progress);
size_t WriteCallback(void* contents, size_t size, size_t nmemb, ProgressData* progressData);
static int ProgressCallback(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded);
void DownloadPackages(const std::string& URL, const std::string& Path);
void QuikResolve(int ArgLen, char *Packages[]);
extern std::string Repo, InstallDIR, PackagesDIR, CacheDIR;

