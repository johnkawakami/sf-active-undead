<div class="bcrumb"><a href="/"><?php echo $GLOBALS['site_crumb']; ?></a> : <a
href="/process/">process</a> : <a href="/process/resources/">making
media</a> : tutorials</div><br>

<div class="hedl"><strong>Audio/Video Encoding Recommendations</strong></div>
<div>The audio and video collectives are developing recommendations for the capture, encoding,
and publishing of media on the web site. </div><br>

<div class="hedl"><strong>Web Tutorial Links</strong></div>

<div><a href="http://www.transom.org/tools/">Transom.org</a> has lots of
helpful audio info, from <a
href="http://www.transom.org/tools/recording_interviewing/200106.microphones.jtowne.html">choosing
a good microphone</a> to working with the latest audio applications. See
also Ron Dexter's film/video/audio <a
href="http://www.rondexter.com/">production tips</a>. We also host <a
href="interviewing.php">Tips on Radio Interviewing</a>, <a 
href="finalcut.php">Final Cut Pro tutorial</a>, and <a 
href="cleaner.php">Cleaner tutorial</a> here.</div><br>

<div class="hedl"><strong>MP3 Audio</strong></div> <div>Dozens of audio
applications are on the web as freeware. Even the top-of-the-line audio
workstation software <a href="http://www.digidesign.com/ptfree/">Pro
Tools</a> is a free, allbeit huge, download.</div><br>

<div>When recording sound onto your computer, it is important to adjust
the output volume of your source so that it doesn't "clip," or overload,
your computer's audio input.  Simple sound recording freeware such as <a
href="http://versiontracker.com/moreinfo.fcgi?id=959&db=mac">Coaster</a>
(mac) can help you set your levels; Coaster also allows you to chop the
recording into separate files as you record.</div><br>

<div>For best results, MP3 audio files containing voice-only material
should be encoded in Mono at a bitrate of up to 64k. Stereo and a higher
bitrate of 128k can be used for files that include music.  When encoding
streaming MP3 files at modem-bitrates (such as 16k), choose a low sampling
rate (such as 16 kHz). For best results (higher quality sound and a
smaller filesize), turn on VBR (variable bit rate) encoding. You should
also remember to type in the title and any other information for the "ID3"
fields. On the Mac side, the free <a
href="http://www.apple.com/itunes/">iTunes</a> is excellent for encoding
MP3s.</div><br>

<div class="hedl"><strong>RealMedia</strong></div>

<div>First of all, you will need an audio or video file that is compatible with <a
href="http://proforma.real.com/rn/tools/producer/">RealProducer</a>, <a href="http://www.terran.com/cleaner/">Media
Cleaner</a>, or any other program that can encode RealMedia files.  The better the quality of your source material (for
example, uncompressed audio), the better the quality of your RealMedia file. RealMedia clips can be encoded as single-rate or
multi-rate (SureStream).  We serve RealMedia via the real time streaming protocol (RTSP), so make sure to choose SureStream;  
the file will be larger, but re-buffering will be reduced to a minimum, and image and sound quality vastly
improved.</div><br>

<div>If you are using the free version of RealProducer, you can only choose two SureStream rates.  We usually recommend 28.8k
and 56k, as the majority of internet users have slow connections. Those using <a
href="http://www.terran.com/products/cleaner/">Media Cleaner Pro</a>, RealProducer Plus, or other software can choose 28.8k,
56k, single ISDN, and dual ISDN. Additional higher bitrates are an excessive waste of disk space and bandwidth.</div><br>

<div>If you are using <b>Cleaner 5</b>, use the Settings Wizard to set up your RealMedia encode: WWW &gt; Real &gt;  
Realtime Streaming &gt; 28.8k, 33.6k Modem; 56k Modem, ISDN (Single); ISDN (Double) &gt; etc.  Click on the frame to
crop your movie as desired.</div><br>

<div class="hedl"><strong>QuickTime</strong></div>

<div>We do not serve QuickTime via a streaming server; instead, progressive download allows full-quality QuickTime video to
be viewed even by those with slow connections.  Users may have to wait for most of the QuickTime movie to download, but it
will often be of higher quality than a streaming RealVideo.  The Sorenson codec offers the best quality (choose "Fast
Start - Compressed Header"); the Cinepak codec is more cross-platform compatible (choose "Fast Start").</div><br>

<div>If you are using <b>Cleaner 5</b>, use the Settings Wizard to set up your QuickTime encode: WWW &gt; QuickTime
&gt; Progressive Streaming (high quality) &gt; T1, Cable Modem, xDSL, LAN ('Broadband') &gt; etc.  Click on the
frame to crop your movie as desired.</div><br>

<div class="hedl"><strong>Other Formats</strong></div> <div>You can also upload Flash (.swf), MPEG (.mpg), and SMIL (.smi)  
files.  Flash movies can include animation, interactivity, and sound.  MPEG video includes the Video CD (MPEG-1) and
DVD-Video (MPEG-2) formats. <a href="http://www.w3.org/AudioVideo/">SMIL</a> ("smile," Synchronized Multimedia Integration 
Language) is similar to HTML, and is
compatible with Internet Explorer 5.5, QuickTime 4.1, RealPlayer 8, and other software.</div><br>

Videographers can use their facilities to edit and upload footage using 
<a href="finalcut.php">Final Cut Pro</a> and <a href="cleaner.php">Media 
Cleaner Pro</a>.</div><br>

<div class="hedl"><strong>Shooting for the Web</strong></div><div>Before you start shooting video for the web, it is
important to understand that most video codecs work by creating a series of "delta frames" - packets of data telling the
viewing application how the current frame differs from the previous frame. The more the image changes from frame to frame,
the more data will have to be crammed into each delta frame.  This means that fewer frames can be sent, and less image detail
can be described for each frame.</div><br><div>So, your goal is to make each frame almost exactly the same as the frame
before!  How do you do this?  There are four simple techniques:</div> <ul> <li><b>Use a tripod.</b> By avoiding shaking, the
background will be almost identical from frame to frame.</li> <li><b>Avoid unnecessary camera movement, including zooms.</b>
Every time you pan or zoom, your web video will drop frames and image quality will degrade.</li> <li><b>Turn off autofocus,
unless you need it.</b> On many cameras, autofocus will perform slight changes on the focus almost constantly.  Each change
of focus will probably change every pixel on the screen - it's just as bad as zooming.  So, if you can, set focus manually
and keep it there.</li> <li><b>Turn off auto iris (aka auto exposure), unless you need it.</b> Likewise, your camera's slight
changes in the iris will cause every pixel in the image to get lighter or darker - it's just as bad as auto-focus.  So, if
you can, set the exposure manually and keep it there.</li> </ul>

<div class="hedl"><strong>Encoding Web Video using RealProducer</strong></div>

<div>If you are using <a href="http://proforma.real.com/rn/tools/producer/">RealProducer</a>, you will definitely want to
turn on several video codec options to improve your image quality: RealVideo 8 (which offers superior video quality compared
to RealVideo G2), 2-pass encoding, variable bit rate (VBR) encoding, and loss protection. Combining 2-pass and VBR (see
definitions of these terms below) delivers a huge improvement in video quality, but don't be surprised if it also doubles
your encoding time!</div><br>

<div>Professional (that is, not free) encoding software allows you, among other things, to re-size your video from full
640x480 pixels down to more web-friendly sizes such as 160x120. If you are using the free RealProducer, you will have to
export your clip at the final intended ("postage stamp") size in <b>uncompressed</b> .avi or .mov format before importing it
into RealProducer. Remember to choose horizontal and vertical resolutions that are both divisible by 4 (e.g. 176x132 but not
180x135), and in a ratio of 4:3 (or 16:9 for widescreen DV) - see <i>Horizontal/Vertical Resolution</i>, below. <a
href="http://www.apple.com/quicktime/download/">QuickTime Pro</a>, $30, is useful for exporting video - see <i>Exporting an
Uncompressed QT movie</i>, below. </div><br>

<div class="hedl"><strong>Media Cleaner</strong></div>

<div><a href="http://www.terran.com/products/cleaner/">Media Cleaner</a> encodes streaming audio and video faster and better
than other programs.  We recommend acquiring it yourself, or visit <a href="http://www.media-alliance.org/">Media
Alliance</a> in San Francisco and use it there; then you can upload your video over their T1 internet connection.</div><br>

<div>Individual captured clips (for example, from an iMovie project's Media folder) can usually be imported directly into
Media Cleaner. This will save you the trouble of exporting from your capture program, unless you need rendered transitions or
titles.</div><br>

<div class="hedl"><strong>Horizontal/Vertical Resolution</strong></div>

<div>Higher resolutions will give you more image detail but lower frame rates (jerkier motion). Talking heads should be
smaller so the image has finer detail and the lips move smoothly; long shots with signs that need to be readable may require
higher resolutions and jerkier motion. Higher resolutions render as a "slide show" effect.</div>

<pre>
  4:3 (standard)  16:9 (widescreen)
144x108          192x108
160x120
176x132
192x144          256x144
208x156
224x168
240x180          320x180
256x192
272x204
288x216          384x216
304x228
320x240
</pre>

<div>These are the standard resolutions.  But if you're using Media Cleaner Pro, don't be afraid to crop your video to a
different aspect ratio! Before compressing for the web, always get rid of any excess frame real estate - for instance, turn
your video into a dramatic widescreen presentation by cropping off the top and/or bottom.</div><br>

<div class="hedl"><strong>Exporting an Uncompressed QT movie</strong></div><div>The free RealProducer cannot resize video
down to a web-ready "postage stamp" size.  You may be able to use the $30 <a
href="http://www.apple.com/quicktime/download/">QuickTime Pro</a> to export video created by your video capture or video
editing application.  Or, such an application may be able to perform a QuickTime export itself (in iMovie, for instance,
click "expert" in the quicktime dialog).  Here are the options to select in the QuickTime export dialog box:</div>

<ul><li>File menu &gt; Export...</li>
<li>click Export: Movie to QuickTime Movie</li>
<li>click Options...</li>
<li>the Video, Allow Transcoding, and Sound boxes should be checked; the Prepare for Internet Streaming box should <b>not</b> be checked.</li>
<li>click Video Settings...</li>
<ul><li>set Compressor to None and Millions of Colors</li>
<li>set Quality to Best</li>
<li>set Motion to 29.97 Frames per second</li>
<li>click OK</li>
</ul><li>If you need to adjust the brightness, color balance, or contrast, click Video Filter...</li>
<ul><li>set the filter you would like to apply, or None, and click OK</li></ul>
<li>click Video Size...</li>
<ul><li>click Use Custom Size; Width: 160 and Height: 120 (or other resolutions in the <i>Horizontal/Vertical Resolution</i> section above)</li>
<li>click OK</li>
</ul><li>click Sound Settings...</li>
<ul><li>Compressor: None</li>
<li>set Rate: to the sample rate of your source material - for DV this will usually be 48.000 kHz</li>
<li>Size: 16 bit</li>
<li>Use: Mono unless your clip could benefit from stereo sound</li>
<li>click OK</li>
</ul><li>click OK</li>
<li>type a .mov file name and click Save</li>
</ul>

<div>At "postage stamp" size, a video clip will take up
about half the space of the same clip in DV format - about 1GB per 10
minutes. It can now be imported into the free RealProducer for creation of a RealMedia (.rm) file.</div><br>

<div class="hedl"><strong>Definitions of 2-pass and VBR</strong></div>
<div>Two-pass encoding increases quality of output video by analyzing video data for transitions and overall complexity before encoding the input video. VBR encoding enables the video codec to vary the bit rate throughout the clip; more bits are spent on high-action scenes, taking away bits from low-action scenes. This drastically improves quality for both narrowband and broadband video. For best results, turn on both of these features, as they complement each other to greatly improve video quality.</div><br>

<div><small>Last updated <?php echo date("r",filemtime($_SERVER['SCRIPT_FILENAME'])); ?></small></div><br>


<div class="bcrumb"><a href="/"><?php echo $GLOBALS['site_crumb']; ?></a> : <a
href="/process/">process</a> : <a href="/process/resources/">making
media</a> : tutorial</div>
