/*! sf-active-js 2015-10-18 */
function makeUrlClickHandler(a,b){return function(){History.pushState(null,"local","?v=cont&stb="+b+"&url=http://la.indymedia.org"+a)}}function formatArticleList(a,b,c){if(null===b)return void console.log("json is null");for(var d='<ul class="articlelist">',e=0;e<b.length;e++)j=b[e],d+='<li id="'+a+"-id-"+j.id+'" class="noselect"><a href="?v=cont&stb='+c+"&url=http://la.indymedia.org"+j.url+'">'+j.title+"</a></li>";return d+="</ul>"}function formatCalendarList(a){if(null===a)return void console.log("json is null");for(var b='<ul class="articlelist">',c=0;c<a.length;c++)j=a[c],b+='<li id="calendar-id-'+j.id+'" class="noselect"><a href="?v=cont&url=http://la.indymedia.org'+j.url+'">'+j.title+"</a><br />&nbsp;<span class='eventdate'>"+j.start+"</span></li>";return b+="</ul>"}function getProxyUrl(a){return document.location.href.match("indymedia.lo")||document.location.href.match("192.168.111.4")?"/js/ws/proxy.php?url="+escape(a):a}function main(a){var b=new URI(document.location.href),c=b.search(!0),d=c.url,e=c.v;e?console.log("v specified, so doing nothing"):d&&""!==d?(console.log("url set, assuming it's content"),History.replaceState(null,"content","/?v=cont&url="+d)):(console.log("no  url specified, changing to thumb"),History.replaceState(null,"thumbscreen","?v=thum"));var f=c.cache;"1"==f&&(localStorage.rsstime=0),layoutModule(a,EmbedVideo())}var IMC=IMC||{};IMC.arrow=function(){var a=$("#arrow");$(document).scrollTop()>200?"none"==a.css("display")&&a.fadeTo("slow",.7):"none"!=a.css("display")&&a.fadeOut("slow")},IMC.activateArrow=function(){IMC.arrowPoller||(IMC.arrowPoller=window.setInterval(IMC.arrow,500)),$("#arrow").on("click",IMC.scrollUp)},IMC.deactivateArrow=function(){$("#arrow").fadeOut(),IMC.arrowPoller&&(window.clearInterval(IMC.arrowPoller),delete IMC.arrowPoller)},IMC.makeScrollUpFrame=function(a){return function(){$(document).scrollTop(a)}},IMC.scrollUp=function(){var a,b=$(document),c=b.scrollTop(),d=0,e=20,f=e*e,g=c-d;for(a=0;e>a;a++)pos=a*a*g/f,window.setTimeout(IMC.makeScrollUpFrame(pos),20*(e-a))},IMC.scrollDown=function(){var a=$(document);a.scrollTop(a.height())},IMC.share={},IMC.share.facebook=function(a,b){return function(){window.open("http://www.facebook.com/sharer.php?u="+a+"&t="+encodeURIComponent(b))}},IMC.share.google=function(a){return function(){window.open("https://plus.google.com/share?url=http://la.indymedia.org/display.php?id="+a)}},IMC.share.twitter=function(a,b){return function(){window.open("http://www.twitter.com/share?text="+encodeURIComponent(b)+"&url=http://la.indymedia.org/display.php?id="+a)}},IMC.share.email=function(a,b){return function(){window.open("mailto:email@example.com?subject="+encodeURIComponent(b)+"&body="+encodeURIComponent(a))}},IMC.getIdFromQuery=function(){var a=History.getState(),b=new URI(a.url),c=URI.parseQuery(b.query()),d=c.url,e=/\/([0-9]+)\.json$/.exec(d),f=e[1];return parseInt(f)},IMC.postComment=function(a){a.preventDefault(),a.stopPropagation();var b=$("#comment-subject").val(),c=$("#comment-text").val(),d=$("#comment-author").val();if(""===b||""===c||""===d)return void alert("No empty fields allowed");var e=$("#editor").attr("data-csrf-token");url="/js/ws/post.php",data={csrf_token:e,author:d,subject:b,text:c,parent_id:IMC.getIdFromQuery()},console.log(data),$.post(url,data,function(a){return location.reload(),!1},"json").done(function(){}).fail(function(){return alert("Post Failed!"),!1}).always(function(){}),window.localStorage.scrollToBottom=1},IMC.hideCommentForm=function(){var a=$("#editor");a.hide(0),$("#disclose").html("&#9654; Add Comment")},IMC.toggleCommentForm=function(){var a=$("#editor");"none"==a.css("display")?(a.slideDown({duration:500,progress:IMC.scrollDown}),$("#disclose").html("&#9660; Add Comment"),$.get("/js/ws/csrf.php",function(a){$("#editor").attr("data-csrf-token",a.csrf_token)},"json")):IMC.hideCommentForm()},IMC.disableCommentDiscloser=function(){$("#editor").addClass("hidden"),$("#disclose").addClass("hidden")},IMC.enableCommentDiscloser=function(){$("#disclose").removeClass("hidden")};var layoutModule=function(d,g){function h(){var a=History.getState(),b=new URI(a.url),c=URI.parseQuery(b.query());switch(c.v){case"cont":c.url&&(r(),J(c.v),d("#comment-author").val(void 0),d("#comment-subject").val(void 0),d("#comment-text").val(void 0),d.getJSON(getProxyUrl(c.url),function(a){a.article.numcomments=a.comments.length,s(a.article),t(a.attachments),x(a.comments),1==window.localStorage.scrollToBottom&&(window.localStorage.scrollToBottom=0,IMC.scrollDown()),"1"==c.stb&&IMC.scrollDown(),IMC.currentURL=c.url}).fail(function(a,b,c){alert("There was an error. Try reloading the page.")}));break;case"loca":case"brea":case"feat":case"publ":case"cale":case"comm":J(c.v);break;case"thum":J(c.v);break;default:J(c.v)}}function j(a,b){}function k(a,b){var c=d("#flag"),e=function(a,b){return function(){d.getJSON("http://la.indymedia.org/qc/report.php?id="+a+"&q="+b+"&format=json&callback=?",function(a){a.moderatorScore?alert("Thank you.  Your mod score is "+a.moderatorScore):alert("Thank you for moderating."),l()}).fail(function(){alert("There was an error in moderation.  It has been logged."),l()})}};return p(),n(),IMC.deactivateArrow(),d("#settingswrapper").fadeIn().on("click",l),d("#flag-fraud").click(e(a,"fraud")),d("#flag-racist").click(e(a,"racist")),d("#flag-genocide").click(e(a,"genocide")),d("#flag-chatter").click(e(a,"chatter")),d("#flag-double").click(e(a,"double")),d("#flag-ad").click(e(a,"ad")),d("#flag-porn").click(e(a,"porn")),c.css("position","fixed").css("bottom","0").css("left","0"),c.slideDown(),!1}function l(){return IMC.activateArrow(),d("#flag").fadeOut(),d("#settingswrapper").fadeOut(),d("#flag-fraud").off(),d("#flag-racist").off(),d("#flag-genocide").off(),d("#flag-chatter").off(),d("#flag-double").off(),d("#flag-ad").off(),d("#flag-porn").off(),d("#flag").slideUp(),!1}function m(a,b,c,e){var f=d("#share");return d("#share-twitter").click(IMC.share.twitter(a,c)),d("#share-facebook").click(IMC.share.facebook(b,c)),d("#share-google").click(IMC.share.google(a)),d("#share-email").click(IMC.share.email(b,c)),p(),l(),IMC.deactivateArrow(),d("#settingswrapper").fadeIn().on("click",n),f.css("position","fixed").css("bottom","0").css("left","0"),f.slideDown(),!1}function n(){return IMC.activateArrow(),d("#share").fadeOut(),d("#settingswrapper").fadeOut(),d("#share-twitter").off(),d("#share-facebook").off(),d("#share-google").off(),d("#share-email").off(),d("#share").slideUp(),!1}function o(){return n(),l(),IMC.deactivateArrow(),d("#settingswrapper").fadeIn().on("click",p),d("#settings").slideDown(),q(),!1}function p(){return d("#settingswrapper").fadeOut(),d("#settings").slideUp(),!1}function q(){var a=["black","white","small","medium","large","sans","serif"];d.map(a,function(a,b){d("#settings-"+a).removeClass("lit")}),1==G&&d("#settings-white").addClass("lit"),2==G&&d("#settings-black").addClass("lit"),1==F&&d("#settings-small").addClass("lit"),2==F&&d("#settings-medium").addClass("lit"),3==F&&d("#settings-large").addClass("lit"),1==H&&d("#settings-sans").addClass("lit"),2==H&&d("#settings-serif").addClass("lit")}function r(){d("#topphoto").html(""),d("#heading").html(""),d("#summary").html(""),d("#author").html(""),d("#article").html(""),d("#attachments").html(""),d("#comments").html("")}function s(a){console.log("insertStory"),a.heading&&d("#heading").html(a.heading),a.summary&&d("#summary").html(a.summary),a.author&&d("#author").html("by "+a.author);var g=d("#article");if(g.html(""),/audio/.test(a.mime_type))g.append('<p><audio controls><source src="'+a.fileurl+'" type="'+a.mime_type+'"></audio></p>');else if(/image/.test(a.mime_type))if(a.image){var h=a.image.medium||a.image.original;d("#topphoto").append('<p class="media"><img class="photo" src="'+h+'"></p>')}else d("#topphoto").append('<p class="media"><img class="photo" src="'+a.linked_file+'"></p>');g.append(a.article),a.link&&g.append('<p><a href="'+a.link+'">'+a.link+"</a></p>"),d("<div/>",{"class":"disc"}).append(b=d("<span/>",{"class":"disc-btn",text:"reply"}),c=d("<span/>",{"class":"disc-btn",html:'<span class="icon flagbutton"></span>'}),e=d("<span/>",{"class":"disc-btn",html:'<span class="icon likebutton"></span>'}),f=d("<span/>",{"class":"disc-btn",text:"share"})).appendTo(d(g)),b.click(function(b){j(a.id,b)}),c.click(function(b){k(a.id,b)}),e.click(function(b){openLike(a.id,b)}),f.click(function(b){m(a.id,a.article_url,Encoder.htmlDecode(a.heading),b)})}function t(a){var e=d("#attachments"),f=0,g='<div id="article-{{i}}" class="article"><h2>{{heading}}</h2><p class="byline">by {{{author}}}<br />{{{format_created}}}</p><p><a href="{{{image.original}}}"><img src="{{{image.medium}}}" class="photo" /></a></p><p>{{{article}}}</p>';e.html(""),a.forEach(function(h){++f,h.i=f,h.article&&(matches=h.article.match(/^by (.*)$/m))&&(h.author=matches[1],h.article=h.article.replace(/^by .*$/m,"")),h.image.medium||(h.image.medium=h.image.original);var i=Mustache.render(g,h);comment=d.parseHTML(i),d("<div/>",{"class":"disc"}).append(h=d("<span/>",{"class":"disc-btn",text:"reply"}),b=d("<span/>",{"class":"disc-btn",html:'<span class="icon flagbutton"></span>'}),c=d("<span/>",{"class":"disc-btn",html:'<span class="icon likebutton"></span>'})).appendTo(d(comment)),h.click(function(b){j(a.id,b)}),b.click(function(b){k(a.id,b)}),c.click(function(b){openLike(a.id,b)}),e.append(comment)})}function u(a){return function(b){j(a,b)}}function v(a){return function(b){k(a,b)}}function w(a){return function(b){openLike(a,b)}}function x(e){var f='<div id="article-{{i}}" class="comment"><h2>{{{heading}}}</h2><p>by {{{author}}}<br />{{{format_created}}}</p>{{{attachment}}}{{{article}}}<p><a href="{{{link}}}">{{{link}}}</a></p></div>',h=d("#comments");h.html("");for(var i=0;i<e.length;i++){var j=e[i];j.i=i,j.article=Encoder.htmlDecode(j.article),console.log(j.article),"text/plain"==j.mime_type&&(j.article=j.article.replace(/\n/gm,"<br />")),/image/.test(j.mime_type)&&(j.attachment="<img src='"+j.image.medium+"' class='photo'>"),j.author=Encoder.htmlDecode(j.author);var k=Mustache.render(f,j);k=g.embedYouTube(k),comment=d.parseHTML(k),d("<div/>",{"class":"disc"}).append(a=d("<span/>",{"class":"disc-btn",text:"reply"}),b=d("<span/>",{"class":"disc-btn",text:"flag"}),c=d("<span/>",{"class":"disc-btn",text:"like"})).appendTo(d(comment)),a.click(u(e.id)),b.click(v(e.id)),c.click(w(e.id)),h.append(comment)}}function y(){var a=document.getElementsByTagName("link");if("undefined"==typeof a[3])window.setTimeout(y,1e3);else if(a[1].href="css/src/color"+G+".css",a[2].href="css/src/font"+H+".css",a[3].href="css/src/fontsize"+F+".css",localStorage)localStorage["imc-js.color"]=G,localStorage["imc-js.font"]=H,localStorage["imc-js.fontsize"]=F;else{var b=new Date;b.setDate(b.getDate()+3600),document.cookie="format="+escape([G,H,F].join(","))+"; expires="+b.toUTCString()}}function z(){if(localStorage)G=localStorage["imc-js.color"],G||(G=2),H=localStorage["imc-js.font"],H||(H=1),F=localStorage["imc-js.fontsize"],F||(F=2);else{var a=document.cookie,b=a.split("; ");for(i=0;i<b.length;i++)if(0===b[i].indexOf("format")){var c=b[i].substr(7).split(",");G=c[0],H=c[1],F=c[2]}}window.setTimeout(y,100)}function A(a){function b(a){var b=d("#id-"+feature[a].id);d.getJSON(getProxyUrl("http://la.indymedia.org"+feature[a].url),function(a){var c=d("<img>");c.load(function(){b.prepend(d("<br>")),b.prepend(c)}),c.attr("src",a.article.linked_file)})}console.log("loaded the headlines"),local=a.local,feature=a.features,calendar=a.calendar,breakingnews=a.breakingnews,latestcomments=a.latestcomments,E=formatArticleList("local",local,0),d("#local").append(E),attachArticleListClickHandler("local",local,0),C=formatArticleList("breaking",breakingnews,0),d("#breakingnews").append(C),attachArticleListClickHandler("breaking",breakingnews,0),calendarCache=formatCalendarList(calendar),d("#calendar").append(calendarCache),attachCalendarListClickHandler(calendar),D=formatArticleList("feature",feature,0),d("#feature").append(D),attachArticleListClickHandler("feature",feature,0),b(0),b(1),b(2),latestCommentsCache=formatArticleList("comment",latestcomments,1),d("#latestcomments").append(latestCommentsCache),attachArticleListClickHandler("comment",latestcomments,1)}var B=0,C=null,D=null,E=null,F=1,G=1,H=1,I={thum:["#thumbscreen","LA Indymedia"],loca:["#local","Local News"],brea:["#breakingnews","Breaking News"],feat:["#feature","Featured Stories"],publ:["#publish","Publish"],cale:["#calendar","Calendar"],comm:["#latestcomments","Latest Comments"],cont:["#content","LA Indymedia"]},J=function(a){(null===a||""===a)&&(a="thum");for(var b in I)b==a?(d(I[b][0]).css("display","block"),d(document).attr("title",I[b][1]),d("#header-title").html(I[b][1]),IMC.hideCommentForm(),"cont"==b||"publ"==b?IMC.enableCommentDiscloser():IMC.disableCommentDiscloser()):d(I[b][0]).css("display","none")};d(document).on({ajaxStart:function(){B++,d("#spinner").attr("src","images/spinner_black.gif")},ajaxStop:function(){B--,0>=B&&d("#spinner").attr("src","images/black.gif")}}),d("#thumbscreenbutton").on("click",function(){History.back()}),d("#blocal").on("click",function(){History.pushState(null,"local","?v=loca")}),d("#bbreaking").on("click",function(){History.pushState(null,"breaking news","?v=brea")}),d("#bcalendar").on("click",function(){History.pushState(null,"calendar","?v=cale")}),d("#bfeatures").on("click",function(){History.pushState(null,"features","?v=feat")}),d("#bpublish").on("click",function(){History.pushState(null,"publish","?v=publ")}),d("#blatestcomments").on("click",function(){History.pushState(null,"latest comments","?v=comm")}),d("#add-comment-button").on("click",function(a){return IMC.postComment(a),!1}),d("#disclose").on("click",function(){IMC.toggleCommentForm()}),d("#settings-close").on("click",function(){return p()}),d("#settings-open").on("click",function(){return o()}),d("#settings-small").on("click",function(){F=1,q(),y()}),d("#settings-medium").on("click",function(){F=2,q(),y()}),d("#settings-large").on("click",function(){F=3,q(),y()}),d("#settings-white").on("click",function(){G=1,q(),y()}),d("#settings-black").on("click",function(){G=2,q(),y()}),d("#settings-serif").on("click",function(){H=2,q(),y()}),d("#settings-sans").on("click",function(){H=1,q(),y()}),History.Adapter.bind(window,"statechange",h),d("#editor").hide(0),h(),d("#publish").append("publish"),d.getJSON(getProxyUrl("http://la.indymedia.org/js/ws/regen.php"),A,function(a){console.log("some kind of error happened")}),z()},attachArticleListClickHandler=function(a,b,c){for(var d=0;d<b.length;d++){var e=$("#"+a+"-id-"+b[d].id);e.on("click",makeUrlClickHandler(b[d].url,c)),e.html(e.contents().text())}},attachCalendarListClickHandler=function(a){for(var b=0;b<a.length;b++){var c=$("#calendar-id-"+a[b].id);c.on("click",makeUrlClickHandler(a[b].url,0));var d=$("#id-"+a[b].id+" a");d.html(d.contents().text())}},EmbedVideo=function(){function a(a){var b=/(http:\/\/www\.)*youtube.com\/watch\?v=([a-zA-Z]{4,16})/,c=[],d=0,e=a.match(b);return e&&(c[d++]='<iframe class="videoPlayer" width="420" height="315" src="http://www.youtube.com/embed/'+e[2]+'" frameborder="0" allowfullscreen></iframe>'),a+c.join()}function b(a){}return{embedYouTube:a,embedDailyMotion:b}};jQuery(main);