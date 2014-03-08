package com.nhncorp.bowling.driver;


import org.junit.*;
import static org.junit.Assert.*;

public class LogOnManagerTest {
	private LogOnManager logOnManager;
	private Employee employee1;
	private Employee employee2;
	private Employee employee3;

	
	@Before
	public void setUp() throws Exception {
		logOnManager = new LogOnManager();
		employee1 = Employee.getUniqueEmployee();
		employee2 = Employee.getUniqueEmployee();
		employee3 = Employee.getUniqueEmployee();

		logOnManager.registerUser("KR10000", "1", employee1);
		logOnManager.registerUser("KR10001", "2", employee2);
		logOnManager.registerUser("KR10002", "3", employee3);
	}

	@After
	public void tearDown() throws Exception {
	}

	@Test (expected = Exception.class)
	public void registerUserTest() throws Exception {
		logOnManager.registerUser("KR10000", "22", employee1);
	}
	
	@Test
	public void getTotalEmployeeIdsTest() {
		EmployeeId[] employeeIds = logOnManager.getTotalEmployeeIds();
		assertEquals(3, employeeIds.length);
	}
	
	@Test
	public void logOnTest() {
		assertTrue(logOnManager.logOn(new String("KR10000"), "1"));
		assertTrue(logOnManager.logOn("KR10001", "2"));
		assertTrue(logOnManager.logOn("KR10002", "3"));
	}
	
	@Test
	public void logOnTest2() {
		assertFalse(logOnManager.logOn(new String("KR10000"), "2"));
		assertFalse(logOnManager.logOn("KR10003", "2"));
		assertFalse(logOnManager.logOn("KR10004", "3"));
	}
	
	@Test
	public void logOutTest() {
		logOnManager.logOn("KR10000", "1");
		assertTrue(logOnManager.logOut("KR10000"));
	}
	
	@Test	
	public void logOutTest2() {
		assertFalse(logOnManager.logOut("KR10000"));
	}
	
	@Test
	public void isLoggedOnTest() {
		logOnManager.logOn("KR10000", "1");
		assertTrue(logOnManager.isLoggedOnUser("KR10000"));
		assertFalse(logOnManager.isLoggedOnUser("KR10001"));
	}
	
	@Test
	public void getEmployeeFromIdTest() throws Exception {
		assertEquals(employee1, logOnManager.getEmployeeFromId("KR10000"));
		assertEquals(employee2, logOnManager.getEmployeeFromId("KR10001"));

	}
	
	@Test (expected = Exception.class)
	public void getEmployeeFromIdTest2() throws Exception {
		logOnManager.getEmployeeFromId("KR10004");

	}
	
	@Test
	public void getIdFromEmployeeIdTest() {
		assertEquals("KR10000", 
				logOnManager.getIdFromEmployeeId(employee1.getEmployeeId()));
		assertEquals("KR10001", 
				logOnManager.getIdFromEmployeeId(employee2.getEmployeeId()));
		assertEquals("KR10002", 
				logOnManager.getIdFromEmployeeId(employee3.getEmployeeId()));
	}
	
	@Test
	public void getEmployeeFromEmployeeIdTest() {
		assertEquals(employee1, 
				logOnManager.getEmployeeFromEmployeeId(employee1.getEmployeeId()));
	}
	
}
