package com.nhncorp.bowling.driver;

import java.util.HashSet;

public class ApplyManager {
	private final HashSet<EmployeeId> totalEmployeeIds;
	private HashSet<EmployeeId> applicantIds;
	
	public ApplyManager(EmployeeId[] totalEmployeeIds) {
		this.totalEmployeeIds = new HashSet<EmployeeId>();
		for (int i = 0; i < totalEmployeeIds.length; i++) {
			this.totalEmployeeIds.add(totalEmployeeIds[i]);
		}
		applicantIds = new HashSet<EmployeeId>();
	}
	
	public boolean apply(EmployeeId applicantId) throws Exception {
		if (totalEmployeeIds.contains(applicantId) == false) {
			throw new Exception();
		} else {
			return applicantIds.add(applicantId);
		}
	}
	
	public boolean checkIfApplied(EmployeeId applicantId) throws Exception {
		if (totalEmployeeIds.contains(applicantId) == false) {
			throw new Exception();
		}
		return applicantIds.contains(applicantId);
	}
	
	public EmployeeId[] getTotalPlayers() {
		return applicantIds.toArray(new EmployeeId[applicantIds.size()]);
	}

}
