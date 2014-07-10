use Hacking
DBCC SHRINKFILE(Hacking_log)
BACKUP LOG Hacking 
DBCC SHRINKFILE(Hacking_log)