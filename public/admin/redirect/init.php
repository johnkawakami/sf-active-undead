<?php // vim:et:ai:ts=4:sw=4
include 'shared/vendor/autoload.php';

ini_set('display_errors', 1);
include('shared/global.cfg');
define('DB_DSN', 'mysql:dbname='.DB_DATABASE.';host='.DB_HOSTNAME);
$pdo = new PDO(DB_DSN, DB_USERNAME, DB_PASSWORD);
$display = 1;

if ($_SESSION['is_editor'] !== true) {
        $goto=urlencode($_SERVER['REQUEST_URI']);
        header("Location: /admin/authentication/authenticate_display_logon.php?goto=$goto");
        exit();
}

