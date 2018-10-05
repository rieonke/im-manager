package cn.rieon.tools.immanager;

public class InputMethodException extends RuntimeException {

  public InputMethodException() {
  }

  public InputMethodException(String message) {
    super(message);
  }

  public InputMethodException(String message, Throwable cause) {
    super(message, cause);
  }

  public InputMethodException(Throwable cause) {
    super(cause);
  }

  public InputMethodException(String message, Throwable cause, boolean enableSuppression,
      boolean writableStackTrace) {
    super(message, cause, enableSuppression, writableStackTrace);
  }
}
