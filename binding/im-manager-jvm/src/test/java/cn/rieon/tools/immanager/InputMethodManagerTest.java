package cn.rieon.tools.immanager;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

public class InputMethodManagerTest {

  private static final String id = "com.apple.keylayout.ABC";
  public static final String TYPE = InputMethodManager.TYPE_OSX;

  @Before
  public void init() {
    InputMethodManager.init(TYPE);
  }

  @Test
  public void getCurrentInputMethod() {
    System.out.println("<<<<<<<<<<<<<<<");
    InputMethod im = InputMethodManager.getCurrentInputMethod();
    System.out.println("Current Input Method: " + im.getId());
  }

  @Test
  public void getAvailableInputMethods() {
    System.out.println("<<<<<<<<<<<<<<<");
    System.out.println("All Available Input Methods: ");
    InputMethod[] aim = InputMethodManager.getAvailableInputMethods();
    for (InputMethod im : aim) {
      System.out.println(im.getId());
    }
  }

  @Test
  public void switchTo() {
    System.out.println("<<<<<<<<<<<<<<<");
    System.out.println("Switch to " + id);
    boolean res = InputMethodManager.switchTo(id);
    assertTrue(res);
  }

  @Test
  public void getInputMethod() {
    System.out.println("<<<<<<<<<<<<<<<");
    InputMethod cim = InputMethodManager.getInputMethod(id);
    assertEquals(id, cim.getId());
    System.out.println("Get specified input method success");
  }
}