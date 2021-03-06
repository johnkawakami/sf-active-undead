    <!-- Babcom: Decentral Anonymous Spamresistant comments on Static Websites.

    Copyright: 2014 Arne Babenhauserheide (http://draketo.de) You are free
    to use this under any license you chose. If you want to be nice,
    release your changes under free software licenses.  -->
    <style type="text/css">
    #babcom-realcomments, #babcom-publicsoneid iframe {
      /*width: 1024px;
      height: 400px;*/
      width: 600px;
      height: 100px;
      border: solid thin black;
    }
    #babcom-needsfreenet {
      width: 600px;
      height: 300px;
      margin-bottom: -1em;
      border: solid thin black;
    }
    #babcom-needsfreenet p, #babcom-needsfreenet blockquote {
      padding-left: 0.5em;
      padding-right: 0.5em;
    }
    </style>
    <!-- TODO: Save the node in a cookie -->
    <hr>
    Freenet/Sone comments box:<br />
    <p>This is a comment system called <a href="http://draketo.de/proj/freecom">babcom</a>, which is a decentralized, anonymous, encrypted, troll-resistant, comment system
    that runs atop <a href="https://freenetproject.org/">Freenet</a>.  These comments are not stored on this IMC server - they exist in the Freenet cloud, and  you 
		must be running Freenet and Sone to see the comments.  Install <a href="https://freenetproject.org/">Freenet</a> and try it out.</p>
 
    <p><button type="button" onclick="loadcomments()">Reload Comments</button> Use Node: <input type="text" name="babcom-node" id="babcom-node" value="http://127.0.0.1:8888" /></p>
    <div id="babcom-needsfreenet"><blockquote><strong>⚙ Babcom is trying to load the comments ⚙</strong></blockquote>
    <p>This textbox will disappear when the comments have been loaded.</p>
    <p>If the box below shows an error-page, you need to install <a href="http://freenetproject.org">Freenet</a> with the <a href="http://freesocial.draketo.de/sone_en.html">Sone-Plugin</a> or set the node-path to your freenet node and click the <tt>Reload Comments</tt> button (or return).</p>
<p>If you see something like <code>Invalid key: java.net.MalformedURLException: There is no @ in that URI! (Sone/search.html)</code>, you need to setup <a href="http://freesocial.draketo.de/sone_en.html">Sone</a> and the <a href="http://freesocial.draketo.de/wot_en.html">Web of Trust</a></p></div>
    <p id="babcom-comments"></p>
    <p id="babcom-nojswarning">If you had Javascript enabled, you would see comments for this page instead of the Sone page of the sites author.</p>
    <iframe id="babcom-publicsoneid" src="http://127.0.0.1:8888/Sone/viewSone.html?sone=6~ZDYdvAgMoUfG6M5Kwi7SQqyS-gTcyFeaNN1Pf3FvY"></iframe>
    <p><em style="color:red">Note: To make a comment which isn’t a reply visible to others here, include the red link below somewhere in the comment text.</em></p>
    <p>Add this link to the comment: <script>document.write(document.title)</script> <span style="color:red" id="babcom-sitelink"></span></p>
    <script type="text/javascript">
            /* @licstart The following is the entire license notice for
            the JavaScript code within this script block (script
            to /script).

            Copyright (C) 2014  Arne Babenhauserheide

            The JavaScript code in this page is free software: you can
            redistribute it and/or modify it under the terms of the GNU
            General Public License (GNU GPL) as published by the Free Software
            Foundation, either version 3 of the License, or (at your option)
            any later version.  The code is distributed WITHOUT ANY WARRANTY;
            without even the implied warranty of MERCHANTABILITY or FITNESS
            FOR A PARTICULAR PURPOSE.  See the GNU GPL for more details.

            As additional permission under GNU GPL version 3 section 7, you
            may distribute non-source (e.g., minimized or compacted) forms of
            that code without the copy of the GNU GPL normally required by
            section 4, provided you include this license notice and a URL
            through which recipients can access the Corresponding Source.   

            As additional permission under GNU GPL version 3 section 7,
            you may use this code under any other free software license,
            including permissive licenses like BSD and MIT.

            @licend  The above is the entire license notice
            for the JavaScript code  within these script block.
            */
    function loadcomments()
    {
    var freenetnode = document.getElementById("babcom-node").value
    var sonesearch = "/Sone/search.html?query="
    var host = window.location.host;
    var path = window.location.pathname;
    // for imc, we delete the .php and the _comment.php#12345
    path = path.replace(/(_comment|).php/,'');
    var postsanchor = "post-results";
    var searchquery = host + path + "#" + postsanchor;
    var comments = document.getElementById("babcom-comments");
    var publicsone = document.getElementById("babcom-publicsoneid");
    var nojswarning = document.getElementById("babcom-nojswarning");
    var needsfreenet = document.getElementById("babcom-needsfreenet");
    var sitelink = document.getElementById("babcom-sitelink");
    /* when the function gets rerun, replace the realcomments instead of adding.*/
    if (document.getElementById("babcom-realcomments")) {
        var cif = document.getElementById("babcom-realcomments");
        cif.src = freenetnode + sonesearch + searchquery;
    } else {
        comments.innerHTML = "<iframe id=\"babcom-realcomments\" src=\"" + freenetnode + sonesearch + searchquery + "\"></iframe>";
    }
    nojswarning.parentNode.replaceChild(document.createElement("p"), nojswarning);
    publicsone.parentNode.replaceChild(document.createElement("span"), publicsone);
    sitelink.innerHTML = "http://" + host + path;
    commentsiframe = document.getElementById("babcom-realcomments");
    commentsiframe.onload = function(){
            needsfreenet.parentNode.replaceChild(document.createElement("p"), needsfreenet);
            commentsiframe.style.width = "600px";
            commentsiframe.style.height = "400px";
        };
    }
    setTimeout(function () { loadcomments(); }, 3000);
    document.getElementById("babcom-node").onkeydown = function(e) {
        var keyCode = e.keyCode || e.which;

       if (keyCode === 13) {
         loadcomments();
       }
    }
    </script>
