<?php
    // vim:set ts=4 sw=4 ai:
    /**
    * A pseudo-captcha class.  It asks a math question and requests
    * an answer.
    */
   
    class Craptcha
    {
        var $a;
        var $b;
        var $answer;
        var $file;
        function Craptcha( $key )
        {
			if (!$key) $key=12345;
            $dir = 'tmp/craptcha';
            if (!is_dir($dir))
            {
                mkdir($dir);
            }
            $this->file = $file = "$dir/$key";
            if (file_exists($file))
            {
                list($this->a,$this->b,$this->answer) =
                    unserialize(file_get_contents($file));
            }
            else
            {
                $this->a = rand(1,5);
                $this->b = rand(1,5);
                $this->answer = $this->a + $this->b;
                $text = serialize(array($this->a,$this->b,$this->answer));
                $fh = fopen($file,'w');
                fwrite($fh, $text);
                fclose($fh);
            }
        }
        function clear()
        {
            if (file_exists($this->file))
                unlink($this->file);
        }
        function answerMatches( $word )
        {
            return ($this->numToString( $this->answer) == strtolower(rtrim(ltrim($word))) or
					($this->answer == strtolower(rtrim(ltrim($word)))));
        }
        function toString()
        {
            return $this->numToString( $this->a ) . ' plus ' .
                   $this->numToString( $this->b );
        }
        function getAnswer()
        {
            return $this->numToString( $this->answer );
        }
        function numToString( $num )
        {
            switch($num)
            {
                case 1: return 'one';
                case 2: return 'two';
                case 3: return 'three';
                case 4: return 'four';
                case 5: return 'five';
                case 6: return 'six';
                case 7: return 'seven';
                case 8: return 'eight';
                case 9: return 'nine';
                case 10: return 'ten';
            }
        }
    }
?>
