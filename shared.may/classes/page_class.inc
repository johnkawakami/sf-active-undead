<?php // vim:et:ai:ts=4:sw=4

class Page extends Cache
{
    // The Page class is used to compile and build pages

    function Page($pageid = '', $settings=null)
    {
        global $template_obj;
        $this->error = "";

		if ($settings['noindex']) $GLOBALS['noindex']=true;
		if ($settings['canonical_url']) $GLOBALS['canonical_url']=$settings['canonical_url'];
		if ($settings['http_status']) $GLOBALS['http_status']=$settings['http_status'];

		$tr = new Translate;
		$tr->create_translate_table('page');

        // Constructor which begins setting page properties
        if (!is_string($pageid)) {
            $this->error = $tr->trans('string_error');
        } else {
            $this->pageid = $pageid;

            // First, check for a template file
            $this->template_file = $pageid . ".tpl";

            if (!file_exists(SF_TEMPLATE_PATH . "/pages/" . $this->template_file)) {
                $this->error = $tr->trans('template_error').$this->template_file ;
            } else {
                $this->template = new FastTemplate(SF_TEMPLATE_PATH . "/pages");

                // Next, check for a script file
                $this->script_file = SF_SHARED_PATH . "/classes/pages/" . $pageid . ".inc";

                if (!file_exists("$this->script_file")) {
                    $this->error = $tr->trans('script_error');
                } else {
                    include_once("$this->script_file");

                    // Next, try to instantiate the script class
                    if (!class_exists($pageid)) {
                        $this->error = $tr->trans('class_error');
                    } else {
                        if ($settings!=null) {
                            $this->page = new $pageid($settings);
                        } else {
                            $this->page = new $pageid();
                        }
                    }
                }
            }
        }   
                         

        // Last, check for a dictionary file
        $this->translate = new Translate();
        $this->translate->create_translate_table($pageid);

        return 1; 
    }

    function get_error()
    {
        if (strlen($this->error) > 0)
        {
            return $this->error;
        } else
        {
            return false;
        }
    }

    function build_page($content_page = '')
    {

        unset ($this->page->forced_template_file);
	
        if (strlen($content_page) > 0 && $this->pageid == "content_page") {
            $this->page->execute($content_page);
        } else {
            $this->page->execute();
        }

        // ugly hack to hoist up values set inside execute when there are 
        // things to alter in the header - johnk
        if (isset($this->page->settings)) {
            $settings = $this->page->settings;
            if ($settings['noindex']) $GLOBALS['noindex']=true;
            if ($settings['canonical_url']) $GLOBALS['canonical_url']=$settings['canonical_url'];
            if ($settings['http_status']) $GLOBALS['http_status']=$settings['http_status'];
        }

        if (isset($this->page->forced_template_file))
        {
            $this->force_new_template($this->page->forced_template_file);
        }

        $this->template->clear_all();
        $this->template->define(array('page' => $this->template_file));
        $defaults = array();

        while(list($key, $value) = each($GLOBALS['dict']))
        {
            $keyid = "TPL_" . strtoupper($key);
            $defaults[$keyid] = $value;
        }

        if (is_array($this->page->tkeys))
        {
            //echo var_dump($this->pagetkeys);
            //while(list($key, $value) = each($this->page->tkeys))
            foreach( $this->page->tkeys as $key=>$value) 
            {
                $keyid = "TPL_" . strtoupper($key);
                $pagevars["$keyid"] = $value;
            }
            $defaults = array_merge($defaults, $pagevars);
        }

		krsort($defaults);

        file_put_contents('/tmp/x', json_encode($defaults, JSON_PRETTY_PRINT));

	    $this->template->assign($defaults);
        $this->template->parse2('CONTENT',"page");
        $this->html = $this->template->fetch2("CONTENT");

        file_put_contents('/tmp/z', $this->html);
		
        return 1; 
    }

    function get_html()
    {
        return $this->html;
    }

    function force_new_template($template_name)
    {
        // forces a new template if you need to do it in mid-stream

        if (!file_exists(SF_TEMPLATE_PATH . "/pages/" . $template_name))
        {
			$tr = new Translate ;
			$tr->create_translate_table('page');
            $this->error = $tr->trans('template_error');
        } else
        {
            $this->template_file = $template_name;
            $this->template = new FastTemplate(SF_TEMPLATE_PATH . "/pages");         
        }
    }
    
} // end class
