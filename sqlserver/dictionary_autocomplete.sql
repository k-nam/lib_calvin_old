IF OBJECT_ID('tempdb.dbo.#temp') IS NOT NULL
   DROP TABLE #temp;
SELECT TOP 50 word, rank INTO #temp
FROM Wordlist.Eng_100K WHERE word LIKE 'ab%' ORDER BY  (CASE WHEN rank IS NULL THEN 1 ELSE 0 END), rank ASC;
SELECT word, rank FROM #temp ORDER BY word;