package com.nhncorp.bowling.driver;


import org.junit.*;

import com.nhncorp.bowling.main.HappyBeanAccount;

import static org.junit.Assert.*;

public class BettingManagerTest {
	private BettingManager bettingManager;
	private final int NUMBER_OF_EMPLOYEE = 100;
	private EmployeeId[] employeeIds;
	private LogOnManager logOnManager;
	
	@Before
	public void setUp() throws Exception {
		employeeIds = new EmployeeId[NUMBER_OF_EMPLOYEE];
		for (int i = 0; i < NUMBER_OF_EMPLOYEE; i++) {
			employeeIds[i] = EmployeeId.getUniqueEmployeeId();
		}
		bettingManager = new BettingManager(employeeIds, new HappyBeanAccount());
		logOnManager = new LogOnManager();
		for (int i = 0; i < NUMBER_OF_EMPLOYEE; i++) {
			logOnManager.registerUser(
					"KR" + i, "KR" + i, 
					new Employee(employeeIds[i], 20, Employee.Gender.Male, "DEV" + i));
		}
	}

	@After
	public void tearDown() throws Exception {
	}

	@Test
	public void bettingTest() throws Exception {
		for (int i = 0; i < NUMBER_OF_EMPLOYEE; i++) {
			assertTrue(bettingManager.betting(employeeIds[i], employeeIds[0]));
		}
	}
	
	@Test
	public void bettingTest2() throws Exception {
		for (int i = 0; i < NUMBER_OF_EMPLOYEE; i++) {
			bettingManager.betting(employeeIds[i], employeeIds[0]);
		}
		for (int i = 0; i < NUMBER_OF_EMPLOYEE; i++) {
			assertFalse(bettingManager.betting(employeeIds[i], employeeIds[0]));
		}		
	}
	
	@Test (expected = Exception.class)
	public void bettingTest3() throws Exception {
		bettingManager.betting(EmployeeId.getUniqueEmployeeId(), employeeIds[0]);
	}
	
	@Test
	public void distributeMoneyTest() throws Exception {
		bettingManager.betting(employeeIds[1], employeeIds[0]);
		bettingManager.betting(employeeIds[2], employeeIds[1]);
		bettingManager.betting(employeeIds[3], employeeIds[1]);
		bettingManager.betting(employeeIds[4], employeeIds[1]);
		bettingManager.distributeMoney(null, logOnManager);
	}
	
	@Test
	public void distributeMoneyTest2() throws Exception {
		bettingManager.betting(employeeIds[1], employeeIds[0]);
		bettingManager.betting(employeeIds[2], employeeIds[1]);
		bettingManager.betting(employeeIds[3], employeeIds[1]);
		bettingManager.betting(employeeIds[4], employeeIds[1]);
		bettingManager.distributeMoney(employeeIds[1], logOnManager);
	}
	
	@Test
	public void distributeMoneyTest3() throws Exception {
		bettingManager.betting(employeeIds[1], employeeIds[0]);
		bettingManager.betting(employeeIds[2], employeeIds[1]);
		bettingManager.distributeMoney(employeeIds[2], logOnManager);
	}
}
