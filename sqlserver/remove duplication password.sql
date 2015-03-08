SELECT Distinct * INTO temptable from [Hacking].[Password].[CommonPasswordUnder20Digit]
delete [Hacking].[Password].[CommonPasswordUnder20Digit]
INSERT INTO [Hacking].[Password].[CommonPasswordUnder20Digit] select * from  temptable
drop table temptable