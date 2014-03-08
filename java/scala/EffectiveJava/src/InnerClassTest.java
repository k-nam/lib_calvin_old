public class InnerClassTest {
	int normalMember;
	static int staticMember = 0;

	private class InnerClass {
		public InnerClass() {
			normalMember = 0;
			staticMember = 0;
		}
	}

	private static class StaticNestedClass {
		public StaticNestedClass() {
			// normalMember = 0; // error; cannot access non-static member
			staticMember = 0;
		}
	}
}
