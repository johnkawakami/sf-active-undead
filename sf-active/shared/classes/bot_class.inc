<?php

class Bot {
	// isbot is set by wapdetect.inc
	var $isHuman;
	function Bot() {
		global $isBot;
		$this->isHuman = !$isBot;
	}
	function isHuman() { return $this->isHuman; }
}
