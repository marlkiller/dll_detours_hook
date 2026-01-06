#include "HookAdapter.h"
#include <Windows.h>
#include <ShlObj.h>
#include <fstream>
#include <vector>
#include <string>
#include "Logger.h" 

void HookAdapter::OnFirstLaunch() {
     MessageBoxW(
        nullptr,
        L"当前为免费开源的破解版本。如您是付费购买所得，请及时举报售卖渠道并给予差评，避免他人受骗。\n\n免责声明：本版本仅供学习与研究用途，开发者不对由此产生的任何后果承担责任。", 
        L"免责声明",
        MB_OK | MB_ICONINFORMATION
    );
}


void HookAdapter::CheckFirstLaunch() {
    // OnFirstLaunch();
}
