SELECT Distinct * INTO temptable from [Hacking].[RainbowTable].[Md5_20Digit_SimpleReverse_10Iter]
delete [Hacking].[RainbowTable].[Md5_20Digit_SimpleReverse_10Iter]
INSERT INTO [Hacking].[RainbowTable].[Md5_20Digit_SimpleReverse_10Iter] select * from  temptable
drop table temptable