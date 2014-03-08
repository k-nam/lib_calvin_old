public class Main {
	public static void main(String[] args) {
		Object innerClass = new InnerClassTest().new InnerClass();
		Object staticNestedClass = new InnerClassTest.StaticNestedClass();
	}
}
