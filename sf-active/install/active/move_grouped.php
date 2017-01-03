<?php
// script to migrate the grouped articles to sf-active
$connection = pg_connect("host=localhost dbname=active_city user=imc_name password=your_password") or die ("postgresql probel");
$mysql_conn=mysql_connect("localhost","db_name","password") or die ("mysql problem");



$orig="select distinct tolink from weblink";
$orig_result = pg_query($connection,$orig);

while ($row_list=pg_fetch_array($orig_result)) {
$query="select w.id, l.fromlink, l.tolink from webcast w, weblink l where (l.tolink=w.id and l.tolink='".$row_list['tolink']."')";
$result=pg_query($query);
print("done 1 row");
while ($row=pg_fetch_array($result)) {
$update="update webcast set parent_id='".$row['id']."',display='t',arttype='attachment' where id='".$row['fromlink']."'";
$ok=mysql_db_query("db_name",$update);
	}
echo $row_list['tolink']."<br>ahum";
}

?>
