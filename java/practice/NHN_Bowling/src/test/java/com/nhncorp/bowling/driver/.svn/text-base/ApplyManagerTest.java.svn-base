package com.nhncorp.bowling.driver;


import org.junit.*;
import static org.junit.Assert.*;

public class ApplyManagerTest {
	private ApplyManager applyManager;
	private final int NUMBER_OF_EMPLOYEE = 100;
	private EmployeeId[] employeeIds;
	
	@Before
	public void setUp() throws Exception {
		employeeIds = new EmployeeId[NUMBER_OF_EMPLOYEE];
		for (int i = 0; i < NUMBER_OF_EMPLOYEE; i++) {
			employeeIds[i] = EmployeeId.getUniqueEmployeeId();
		}
		applyManager = new ApplyManager(employeeIds);
	}

	@After
	public void tearDown() throws Exception {
	}

	// Helper function
	public void applyAll() throws Exception {
		for (int i = 0; i < NUMBER_OF_EMPLOYEE; i++) {
			applyManager.apply(employeeIds[i]);
		}
	}
	
	@Test
	public void applyTest() throws Exception {
		for (int i = 0; i < NUMBER_OF_EMPLOYEE; i++) {
			assertTrue(applyManager.apply(employeeIds[i]));
		}
	}
	
	@Test
	public void applyTes2() throws Exception {
		applyAll();
		for (int i = 0; i < NUMBER_OF_EMPLOYEE; i++) {
			assertFalse(applyManager.apply(employeeIds[i]));			
		}
	}
	
	@Test (expected = Exception.class)
	public void applyTest3() throws Exception {
		EmployeeId applicantId = EmployeeId.getUniqueEmployeeId();
		applyManager.apply(applicantId);
	}
	
	@Test
	public void checkIfAppliedTest() throws Exception {
		for (int i = 0; i < NUMBER_OF_EMPLOYEE; i++) {
			assertFalse(applyManager.checkIfApplied(employeeIds[i]));
		}
	}
	
	@Test
	public void checkIfAppliedTest2() throws Exception {
		applyAll();
		for (int i = 0; i < NUMBER_OF_EMPLOYEE; i++) {
			assertTrue(applyManager.checkIfApplied(employeeIds[i]));
		}
	}
	
	@Test (expected = Exception.class)
	public void checkIfAppliedTest3() throws Exception {
		applyManager.checkIfApplied(EmployeeId.getUniqueEmployeeId());
	}

}
