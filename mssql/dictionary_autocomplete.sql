drop table #temp;
select  top 50 word, rank  into #temp
from Wordlist.Eng_100K where word like 'ta%' order by  (case when rank is null then 1 else 0 end), rank asc;

select word, rank from #temp order by word;