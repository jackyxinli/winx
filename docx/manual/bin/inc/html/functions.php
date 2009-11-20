<?php

function html_header($title) { echo
"<HEAD>
	<TITLE>$title</TITLE>
	<SCRIPT SRC=\"$RES_PATH/langref.js\"/>
	<LINK REL=\"STYLESHEET\" HREF=\"$RES_PATH/backsdk4.css\"/>
</HEAD>
"; }

function header_bar($category) { echo
"<TABLE CLASS=\"buttonbarshade\" CELLSPACING=\"0\">
	<TR>
		<TD>&#x20;</TD>
	</TR>
</TABLE>
<TABLE CLASS=\"buttonbartable\" CELLSPACING=\"0\">
	<TR ID=\"hdr\">
		<TD CLASS=\"runninghead\" NOWRAP=\"NOWRAP\">$category</TD>
	</TR>
</TABLE>
<H1>$title</H1>
"; }

function extended_text($rtf)
{
	echo "$rtf";
}

function topic_start($category, $title, $brief)
{
	header_bar($categroy);
	echo "<H1>$title</H1>";
	extended_text($brief);
}

function show_args($args)
{
	if (!isset($args))
		return;
		
	echo "<H4>参数说明</H4><DL>";
	for ($args as $arg)
	{
		echo "<DT>";
		if ($arg->attr)
			echo "[$arg->attr] ";
		echo "<I>$arg->name</I></DT><DD>";
		extended_text($arg->body);
		echo "</DD>";
	}
	echo "</DL>\n";
}

?>
