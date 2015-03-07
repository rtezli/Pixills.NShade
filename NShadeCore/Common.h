#include "defines.h"

#include "wrl.h"
#include "exception"
#include "memory"
#include "vector"
#include "iostream"
#include "fstream"
#include "string"

#include "targetver.h"
#include "windows.h"

#include "ppltasks.h"
using namespace concurrency;

#include "rx.hpp"
namespace rx = rxcpp;
namespace rxsc = rxcpp::schedulers;
namespace rxsub = rx::subjects;

#include "fbxsdk.h"
#include "fbxsdk/scene/fbxaxissystem.h"

#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"

#include "d3d11.h"
#include "dxgi.h"
#include "directxmath.h"
#include "dinput.h"

#include "debug.h"
#include "deviceresources.h"
#include "types.h"
#include "file.h"
#include "objparser.h"
#include "fbxreader.h"
#include "constantbufferprovider.h"

using namespace std;