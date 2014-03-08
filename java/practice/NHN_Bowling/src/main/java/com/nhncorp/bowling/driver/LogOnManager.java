package com.nhncorp.bowling.driver;

import java.util.HashMap;
import java.util.HashSet;

public class LogOnManager {
	private HashMap<String, String> idToPassWordMap;
	private HashMap<String, Employee> idToEmployeeMap;
	private HashMap<EmployeeId, String> employeeToIdMap;
	private HashSet<String> loggedOnIdSet;
	
	public LogOnManager() {
		idToPassWordMap = new HashMap<String, String>();
		idToEmployeeMap = new HashMap<String, Employee>();
		employeeToIdMap = new HashMap<EmployeeId, String>();
		loggedOnIdSet = new HashSet<String>();		
	}
	
	public void registerUser(String id, String password, Employee employee) throws Exception {
		if (idToPassWordMap.containsKey(id)) {
			throw new Exception();
		}
		idToPassWordMap.put(id, password);
		idToEmployeeMap.put(id, employee);
		employeeToIdMap.put(employee.getEmployeeId(), id);
	}
	
	public EmployeeId[] getTotalEmployeeIds() {
		EmployeeId[] employeeIds = new EmployeeId[idToEmployeeMap.size()];
		Employee[] employees = 
			idToEmployeeMap.values().toArray(new Employee[idToEmployeeMap.size()]);
		for (int i =0; i < employeeIds.length; i++) {
			employeeIds[i] = employees[i].getEmployeeId();
		}
		return employeeIds;
	}
	
	public boolean isLoggedOnUser(String id) {
		if (loggedOnIdSet.contains(id)) {
			return true;
		} else {
			return false;
		}
	}

	public boolean logOn(String id, String password) {
		if (idToPassWordMap.containsKey(id) == true &&
					idToPassWordMap.get(id).equals(password)) {
			loggedOnIdSet.add(id);
			return true;
		} else {
			return false;
		}
	}
	
	public boolean logOut(String id) {
		if (loggedOnIdSet.contains(id) == true) {
			loggedOnIdSet.remove(id);
			return true;
		} else {
			return false;
		}
	}
	
	public Employee getEmployeeFromId(String id) throws Exception {
		if (idToEmployeeMap.containsKey(id) == true) {
			return idToEmployeeMap.get(id);
		} else {
			throw new Exception();
		}
	}
	
	public String getIdFromEmployeeId(EmployeeId employeeId) {
		return this.employeeToIdMap.get(employeeId);
	}
	
	public Employee getEmployeeFromEmployeeId(EmployeeId employeeId) {
		return idToEmployeeMap.get(getIdFromEmployeeId(employeeId));
	}
}
