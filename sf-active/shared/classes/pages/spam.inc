<?php

// Class for refresh page

class spam extends Page
{

    function spam()
    {
        // Class constructor, does nothing
        return 1;
    }

    function execute()
    {
	$tr=new Translate();
	$this->tkeys['spam_log']=$tr->trans('adminiplog');
	$this->tkeys['spam_blocked']=$tr->trans('adminnextip');
	$this->tkeys['spam_blocklog']=$tr->trans('adminipblock');
    }

} // end class
?>
