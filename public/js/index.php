<?php // vim:et:ai:ts=4:sw=4
    include('shared/global.cfg');
    echo "<!DOCTYPE html><html><head>";
    $url = filter_input(INPUT_GET, 'url', FILTER_SANITIZE_STRING);
    if (preg_match('/(\/news\/\d\d\d\d\/\d\d\/\d+)\.json/', $url, $matches)) {
        $canonical = SF_ROOT_URL. $matches[1].".php";
        echo "<link rel=\"canonical\" href=\"$canonical\" />";
    } 
?>
<style type="text/css">* { box-sizing: border-box; }
        html { 
            overflow-y: scroll;
            position: relative;
            min-height: 100%;
        }
        body {
            padding: 0;
            margin: 0;
        }
        .thumb {
            display: block;
            width: 98%;
            padding-top: 20px;
            height: 60px;
            margin: 6px;
            text-align: center;
            position: relative;
            -android-border-radius: 5dip;
            border-radius: 3px;
        }
        #splash {
            width: 100%;
            height: 1080px;
        }</style>

<link rel="apple-touch-icon" href="images/ios-icon.png"><meta name="viewport" content="width=device-width,user-scalable=0,initial-scale=1,maximum-scale=1"><script type="text/javascript" src="vendor/jquery.min.js"></script><script type="text/javascript" src="js/libs.js"></script><script type="text/javascript" src="js/sf-active-js.js"></script><script>jQuery(document).ready( function () {
            IMC.activateArrow();
            $('#splash').hide();
        });</script></head><body><div id="splash"></div><div id="topbar"><table border="0" cellpadding="0" cellspacing="0" width="100%"><tr><td width="53"><span id="thumbscreenbutton" class="icon"></span></td><td style="color:white" align="center"><img id="spinner" src="images/black.gif" width="16" height="16" align="absmiddle"> <span id="header-title">la.indymedia.org</span></td><td align="right" width="53"><span id="settings-open" class="icon"></span></td></tr></table></div><div id="local"></div><div id="breakingnews"></div><div id="feature"></div><div id="calendar"></div><div id="publish"></div><div id="latestcomments"></div><div id="content" class="content region"><p id="topphoto"></p><h1 id="heading"></h1><p id="author"></p><p id="summary"></p><div id="article"></div><div id="attachments"></div><div id="comments"></div></div><div id="thumbscreen" style="display:none"><ul style="margin:0; padding: 0"><li id="bfeatures" class="thumb one"><span class="label">FEATURES</span></li><li id="blocal" class="thumb two"><span class="label">LOCAL NEWS</span></li><li id="bbreaking" class="thumb three"><span class="label">BREAKING NEWS</span></li><li id="bcalendar" class="thumb four"><span class="label">CALENDAR</span></li><li id="bpublish" class="thumb five"><span class="label">PUBLISH</span></li><li id="blatestcomments" class="thumb six"><span class="label">LATEST COMMENTS</span></li></ul></div><div id="settingswrapper"></div><div id="settings"><table border="0"><tr><td id="settings-small"><small>A</small></td><td id="settings-medium">A</td><td id="settings-large"><big>A</big></td></tr><tr><td id="settings-white">xyz</td><td id="settings-black" style="background-color: black; color: white">xyz</td><td class="none"></td></tr><tr><td id="settings-serif" style="font-family: Times">serif</td><td id="settings-sans" style="font-family: Helvetica,Arial">sans</td><td class="none"></td></tr></table></div><div id="flag" class="buttons"><table border="0"><tr><td id="flag-fraud">misinformation/fraud</td></tr><tr><td id="flag-racist">racist/sexist</td></tr><tr><td id="flag-genocide">genocide</td></tr><tr><td id="flag-chatter">chatter</td></tr><tr><td id="flag-double">double post</td></tr><tr><td id="flag-ad">commercial/ad</td></tr><tr><td id="flag-porn">porn</td></tr></table></div><div id="share" class="buttons"><table border="0"><tr><td id="share-twitter">Twitter</td></tr><tr><td id="share-facebook">Facebook</td></tr><tr><td id="share-google">Google+</td></tr><tr><td id="share-email">Email</td></tr></table></div><div id="disclose" class="content">&#9654; Add Comment</div><div id="editor" class="content"><label>Subject</label><br><input id="comment-subject" name="subject" type="text" size="40" placeholder="Subject"><br><label>Author</label><br><input id="comment-author" name="author" type="text" size="20" placeholder="Your Name"><br><textarea id="comment-text" placeholder="enter your comment here"></textarea><br><input id="add-comment-button" type="button" value="post"></div><br><br><br><br><div id="arrow"><img src="images/scrolltop.png"></div><script><!-- css loader -->
      var cb = function() {
          var h = document.getElementsByTagName('head')[0]; 
          function insert(href) {
            var l = document.createElement('link'); l.rel = 'stylesheet';
            l.href = href;
            h.parentNode.insertBefore(l, h);
          }
          insert('css/main.css');
          insert('css/src/color2.css');
          insert('css/src/font1.css');
          insert('css/src/fontsize2.css');
      };
      var raf = requestAnimationFrame || mozRequestAnimationFrame ||
          webkitRequestAnimationFrame || msRequestAnimationFrame;
      if (raf) raf(cb);
      else window.addEventListener('load', cb);</script></body></html>
