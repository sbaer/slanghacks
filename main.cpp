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
    if (0 == hSlang)
      return 1;

    CREATEGLOBALSESSIONPROC SlangStart = (CREATEGLOBALSESSIONPROC)GetProcAddress(hSlang, "slang_createGlobalSession");

    slang::IGlobalSession** gs = globalSession.writeRef();
    SlangStart(SLANG_API_VERSION, gs);

    slang::SessionDesc sessionDesc = {};
    globalSession->createSession(sessionDesc, session.writeRef());

    slang::TargetDesc targetDesc = {};
    targetDesc.format = SlangCompileTarget::SLANG_DXBC; // FXC-compatible .cso
    targetDesc.profile = globalSession->findProfile("ps_5_0");
    targetDesc.flags = 0;

    slang::ICompileRequest* request = nullptr;
    globalSession->createCompileRequest(&request);

    int unit = request->addTranslationUnit(SLANG_SOURCE_LANGUAGE_SLANG, nullptr);

    const char* source = R"(
struct VS_OUTPUT
{
  float4 position : SV_POSITION;
};

[shader("fragment")]
float4 fragment_shader_main(VS_OUTPUT input) : SV_TARGET
{
  return float4(1,0,0,1);
}
)";
    request->addTranslationUnitSourceString(unit, nullptr, source);
//    slang_addSourceFile(request, unit, inputPath);
    request->setCodeGenTarget(SlangCompileTarget::SLANG_DXBC);
    request->setTargetProfile(0, targetDesc.profile);
    request->addEntryPoint(unit, "fragment_shader_main", SlangStage::SLANG_STAGE_FRAGMENT);
    SlangResult compileResult = request->compile();
    if (compileResult != SLANG_OK)
    {
      std::cerr << "Slang compilation failed:\n";
      std::cerr << request->getDiagnosticOutput() << "\n";
      return 1;
    }

    size_t blobSize = 0;
    const void* blob = request->getEntryPointCode(0, &blobSize);
    const char* outputPath = "blobout.cso";
    std::ofstream out(outputPath, std::ios::binary);
    out.write(reinterpret_cast<const char*>(blob), blobSize);
    out.close();
    std::cout << "Compiled to: " << outputPath << "\n";
    return 0;
}
