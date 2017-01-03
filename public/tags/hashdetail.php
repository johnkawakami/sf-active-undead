<?php
/*
 * This is part of a tool to help discover duplicate messages.
 *
 * Scans the hashes and finds duplicate candidates. Saves results
 * into the duplicates table.
 *
 */
ini_set('display_errors', 1);

session_start();
include_once("tags.lib.php");

try {
    $db = new PDO("mysql:host=".DB_HOSTNAME.";dbname=".DB_DATABASE.";charset=utf8", DB_USERNAME, DB_PASSWORD);
} catch(PDOException $e) {
	$errors[] = $e->getMessage();
	print $e->getMessage();
}

if ($_SERVER['REQUEST_METHOD']=='GET') {
	$hash = $_GET['id'];
	$resolved = false;
} else {
	$id = $_POST['id'];
	$hash = $_POST['hash'];
	$action = $_POST['action'];
	if (!$action) {
			$resolved = true;
	}
}
$id = filter_var($id, FILTER_VALIDATE_INT);

$hash = filter_var($hash, FILTER_VALIDATE_REGEXP,
			array(
				'options'=>array(
					'default' => NULL,
					'regexp' => '/^[a-f0-9]+$/i'
				)
			)
		);

if ($action == 'hide') {
	hideone($id);
}
function hideone($id) {
    global $db;
	$st = $db->prepare("UPDATE webcast SET display='f', comments='duplicate' WHERE id=?");
	$st->execute(array($id));
	$st->closeCursor();
}
if ($action == 'hideall') {
	hideall($hash);
}
function hideall($hash) {
	global $db;
	$stmt = $db->prepare("SELECT article_id FROM article_content_hash WHERE content_hash=?");
	$stmt->execute(array($hash));
	while($row = $stmt->fetch()) {
		$id = $row['article_id'];
		$st = $db->prepare("UPDATE webcast SET display='f' WHERE id=?");
		$st->execute(array($id));
		$st->closeCursor();
	}
}

$stmt = $db->prepare("SELECT article_id FROM article_content_hash WHERE content_hash=?");
$stmt->execute(array($hash));

$display_states = array( 't'=>'other', 'f'=>'hidden', 'l'=>'local', 'g'=>'global');
echo "<h1>List of articles for $hash</h1>";
echo "<table>";
$count = 0;
$count_hidden = 0;
$hidden = array();
while($row = $stmt->fetch()) {
	$id = $row['article_id'];
	$st = $db->prepare("SELECT * FROM webcast WHERE id=?");
	$st->execute(array($id));
	$article = $st->fetch();
	$st->closeCursor();
	$heading = htmlspecialchars($article['heading']);
	$author = htmlspecialchars($article['author']);
	$display = $display_states[$d = $article['display']];
	$numcomment = $article['numcomment'];
	if ($d=='f') {
		$hidden[$id] = $numcomment;
		$count_hidden++;
		$link = "<a href='/news/hidden.php?id=".$id."'>$heading</a>";
	} else {
		$link = "<a href='/display.php?id=".$id."'>$heading</a>";
	}
	?>
	<tr><td><?=$id?></td><td><?=$display?></td>
	<td><?=$numcomment?></td>
	<td><?=$link?></td>
	<td><?=$author?></td>
    <td><form method="post">
		<input type="hidden" name="id" value="<?=$id?>" />
		<input type="hidden" name="hash" value="<?=$hash?>" />
		<input type="submit" name="action" value="hide" />
		</form>
    </td>
    </tr>
    <?   
			$count++;
}
echo "</table>";

if (in_array(ltrim(rtrim($author)), array('judith mpls','UPI'))) {
	hideall($hash);
	$count_hidden = $count;
}
if (in_array(ltrim(rtrim($author)), array('Patrice Faubert','AJLPP-USA','JFAV'))) {
	hideone($id);
	$count_hidden++;
}

if ($count == $count_hidden) {
	echo "<p>All the articles are already hidden.";
	echo " I'll assume they are all spam.";
	echo " I'm going to mark this hash as \"resolved\".</p>";
	$st = $db->prepare("UPDATE duplicates SET resolved=1 WHERE content_hash=?");
	$st->execute(array($hash));
	?><p><a href="listdupe.php">Return to the list of hashes</a></p>
    <script>window.location.href="listdupe.php";</script>
	<?
}

if (($count - 1) == $count_hidden)  {
	foreach($hidden as $id=>$numcomment) {
		$st = $db->prepare("UPDATE webcast SET comments='duplicate' WHERE id=?");
		$st->execute(array($id));
	}
	$st = $db->prepare("UPDATE duplicates SET resolved=1 WHERE content_hash=?");
	$st->execute(array($hash));
	?>
    <script>window.location.href="listdupe.php";</script>
	<?php
		exit();
}

if ( !$resolved ) {
	?><p>Are these all okay?</p>
	<form method="post">
    <input type="hidden" value="<?=$hash ?>" name="hash" />
	<input type="submit" value="All are OK" /></form>
	</form>
	<form method="post">
    <input type="hidden" value="<?=$hash ?>" name="hash" />
    <input type="hidden" value="hideall" name="action" />
	<input type="submit" value="This is Spam. Hide them all." /></form>
	</form>
	<?
		exit();
}
if ( $resolved ) {
	$st = $db->prepare("UPDATE duplicates SET resolved=1 WHERE content_hash=?");
	$st->execute(array($hash));
	?>
    <script>window.location.href="listdupe.php";</script>
	<?
		exit();
}
