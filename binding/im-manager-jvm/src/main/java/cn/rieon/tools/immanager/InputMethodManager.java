package cn.rieon.tools.immanager;

import java.io.File;
import java.net.URL;

public class InputMethodManager {

  public static final String TYPE_OSX = "osx";
  public static final String TYPE_IBUS = "ibus";
  public static final String TYPE_FCITX = "fcitx";
  private static String LIB_FILE_NAME;

  public static void init(String type) {

    switch (type) {

      case TYPE_OSX:
        LIB_FILE_NAME = "lib/libim_manager_jvm_osx.dylib";
        break;
      case TYPE_IBUS:
        LIB_FILE_NAME = "lib/libim_manager_jvm_ibus.so";
        break;
      default:
        throw new InputMethodException("仅支持OSX和IBus");
    }

    String lib = null;

    URL resource = InputMethodManager.class
        .getResource(InputMethodManager.class.getSimpleName() + ".class");

    ClassLoader classLoader = Thread.currentThread().getContextClassLoader();

    if ("jar".equals(resource.getProtocol())) {
      //inside jar
    }

    if ("file".equals(resource.getProtocol())) {
      URL url = classLoader.getResource(LIB_FILE_NAME);
      if (url != null) {
        String path = url.getPath();
        File file = new File(path);
        if (file.exists() && file.isFile()) {
          lib = file.getAbsolutePath();
        }
      }
    }

    if (lib == null) {
      throw new InputMethodException("加载本地库文件失败！库文件路径为空！");
    }

    System.load(lib);

  }

  public static native InputMethod getCurrentInputMethod();

  public static native InputMethod[] getAvailableInputMethods();

  public static native boolean switchTo(String id);

  public static native InputMethod getInputMethod(String id);

}
