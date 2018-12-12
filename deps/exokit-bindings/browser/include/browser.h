#ifndef _BROWSER_H_
#define _BROWSER_H_

#ifndef LUMIN
#include <desktop/browser-desktop.h>
#else
#include <magicleap/browser-ml.h>
#endif

#include <webgl.h>

#include <v8.h>
#include <node.h>
#include <nan.h>

#include <defines.h>

#include <chrono>
#include <deque>
#include <thread>
#include <mutex>
#include <functional>

using namespace std;
using namespace v8;
using namespace node;

namespace browser {

#ifndef LUMIN
typedef CefRefPtr<CefBrowser> EmbeddedBrowser;
#else
typedef Servo2D *EmbeddedBrowser;
#endif

enum class EmbeddedKeyModifiers {
  SHIFT,
  CTRL,
  ALT,
};

// Browser

class Browser : public ObjectWrap {
public:
  static Handle<Object> Initialize(Isolate *isolate);

protected:
  Browser(WebGLRenderingContext *gl, int width, int height, const std::string &url);
  ~Browser();

  static NAN_METHOD(New);
  static NAN_METHOD(UpdateAll);
  static NAN_METHOD(Load);
  static NAN_GETTER(WidthGetter);
  static NAN_SETTER(WidthSetter);
  static NAN_GETTER(HeightGetter);
  static NAN_SETTER(HeightSetter);
  static NAN_GETTER(OnLoadStartGetter);
  static NAN_SETTER(OnLoadStartSetter);
  static NAN_GETTER(OnLoadEndGetter);
  static NAN_SETTER(OnLoadEndSetter);
  static NAN_GETTER(OnLoadErrorGetter);
  static NAN_SETTER(OnLoadErrorSetter);
  static NAN_GETTER(OnConsoleGetter);
  static NAN_SETTER(OnConsoleSetter);
  static NAN_GETTER(OnMessageGetter);
  static NAN_SETTER(OnMessageSetter);
  static NAN_METHOD(Back);
  static NAN_METHOD(Forward);
  static NAN_METHOD(Reload);
  static NAN_METHOD(SendMouseMove);
  static NAN_METHOD(SendMouseDown);
  static NAN_METHOD(SendMouseUp);
  static NAN_METHOD(SendMouseWheel);
  static NAN_METHOD(SendKeyDown);
  static NAN_METHOD(SendKeyUp);
  static NAN_METHOD(SendKeyPress);
  static NAN_METHOD(RunJs);
  static NAN_METHOD(PostMessage);
  static NAN_GETTER(TextureGetter);
  void load(const std::string &url);
  void loadImmediate(const std::string &url, NATIVEwindow *window = nullptr, int width = 0, int height = 0);
  // void resize(int w, int h);
protected:
  WebGLRenderingContext *gl;
  GLuint tex;
  int textureWidth;
  int textureHeight;
  
  /* LoadHandler *load_handler_;
  DisplayHandler *display_handler_;
  RenderHandler *render_handler_; */
  // std::map<CefBrowser *, BrowserClient *> clients;
  EmbeddedBrowser browser_;

  Nan::Persistent<Function> onloadstart;
  Nan::Persistent<Function> onloadend;
  Nan::Persistent<Function> onloaderror;
  Nan::Persistent<Function> onconsole;
  Nan::Persistent<Function> onmessage;
};

// helpers

void QueueOnBrowserThread(std::function<void()> fn);

void RunOnMainThread(std::function<void()> fn);
void QueueOnMainThread(std::function<void()> fn);
void MainThreadAsync(uv_async_t *handle);

// variables

extern bool embeddedInitialized;
extern std::thread browserThread;

extern uv_sem_t constructSem;
extern uv_sem_t mainThreadSem;
extern uv_sem_t browserThreadSem;

extern std::mutex browserThreadFnMutex;
extern std::deque<std::function<void()>> browserThreadFns;

extern uv_async_t mainThreadAsync;
extern std::mutex mainThreadFnMutex;
extern std::deque<std::pair<std::function<void()>, bool>> mainThreadFns;

}

#endif
