#include "slang/include/slang.h"
#include "slang/include/slang-com-helper.h"
#include "slang/include/slang-com-ptr.h"

#include <iostream>
#include <fstream>
#include "windows.h"


typedef void (CALLBACK* CREATEGLOBALSESSIONPROC)(SlangInt apiVersion, slang::IGlobalSession** outGlobalSession);

int main(int argc, char** argv)
{
    Slang::ComPtr<slang::ISession> session;
    Slang::ComPtr<slang::IGlobalSession> globalSession;

    HMODULE hSlang = LoadLibrary("slang.dll");
    CREATEGLOBALSESSIONPROC SlangStart = (CREATEGLOBALSESSIONPROC)GetProcAddress(hSlang, "slang_createGlobalSession");

    slang::IGlobalSession** gs = globalSession.writeRef();
    SlangStart(SLANG_API_VERSION, gs);

    slang::SessionDesc sessionDesc = {};
    globalSession->createSession(sessionDesc, session.writeRef());

    slang::TargetDesc targetDesc = {};
    targetDesc.format = SLANG_DXBC; // FXC-compatible .cso
    targetDesc.profile = globalSession->findProfile("ps_5_0");
    targetDesc.flags = 0;

    slang::ICompileRequest* request = nullptr;
    globalSession->createCompileRequest(&request);

    int unit = request->addTranslationUnit(SLANG_SOURCE_LANGUAGE_SLANG, nullptr);

    /*
    slang_addSourceFile(request, unit, inputPath);
    slang_setTarget(request, 0, SLANG_BINARY_FORMAT_DXBC);
    slang_setTargetProfile(request, 0, targetDesc.profile);
    slang_setEntryPoint(request, unit, "main", SLANG_STAGE_FRAGMENT);

    if (slang_compile(request) != SLANG_OK)
    {
        std::cerr << "Slang compilation failed:\n";
        std::cerr << slang_getDiagnosticOutput(request) << "\n";
        return 1;
    }

    size_t blobSize = 0;
    const void* blob = slang_getEntryPointCode(request, 0, 0, &blobSize);

    std::ofstream out(outputPath, std::ios::binary);
    out.write(reinterpret_cast<const char*>(blob), blobSize);
    out.close();
*/
    //std::cout << "Compiled to: " << outputPath << "\n";
    return 0;
}
