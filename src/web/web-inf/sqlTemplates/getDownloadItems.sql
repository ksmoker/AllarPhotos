#if(!$fromDate)
	select * from findEventItems('download','$!category','$!value',#if($date)'$sql.calToString($date,"yyyy-MM-dd")'#else '1970-01-01'#end)
#else
	select * from findEventItemsDateRange('download','$!category','$!value',#if($fromDate)'$sql.calToString($fromDate,"yyyy-MM-dd")'#else '1970-01-01'#end,#if($toDate)'$sql.calToString($toDate,"yyyy-MM-dd")'#else '2100-01-01'#end)
#end