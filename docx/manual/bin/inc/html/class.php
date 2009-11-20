<?php

function class_decl($classes)
{
	echo "<PRE class=\"syntax\">\n"
	if (!is_array($classes))
		$classes = array($classes);
	foreach ($classes as $class)
	{
		echo "<B>";
		if (isset($class->template))
			echo "$class->template->header<BR>";
		$class = $class->class;
		echo "$class->keyword $class->name";
		if ($class->bases)
		{
			echo " : <BR>\n";
			foreach ($class->bases as $i => $base)
			{
				echo "&#x20;&#x20;";
				if ($i != 0)
					echo ",<BR/>";
				if ($base->access)
					echo "$base->access ";
				echo $base->name;
			}
		}
		echo ";<BR/></B>";
	}
	echo "</PRE>\n";
}

function show_ctors($class, $base)
{
	foreach ($class->sentences as $s)
	{
		if (isset($s->comment))
			$ctordoc = $s->comment;
		else
		{
			if (isset($s->ctor))
			{
				$ctors[] = $ctordoc;
				$ctors[] = $s->ctor;
			}
			unset($ctordoc);
		}
	}
	
	return show_index($class, $ctors, array(
		"title" => "构造函数", "name" => "Constructor", "desc" => "Description"),
		$base . "_$class->name");
}

function show_methods($class, $base)
{
	show_fntable($class, array(
		"fn" => "member", "title" => "方法列表", "name" => "Method", "desc" => "Description"),
		$base . "_$class->name");
}

function show_class()
{
}

?>

<HTML>
<?php
	$title = "todo";
	html_header($title);
?>
<BODY TOPMARGIN="0">
<?php
	topic_start($category, $title, $brief);
	class_decl($classes);
	show_args($args);
	show_desc($desc);
	show_remark($remark);
?>
	<xsl:apply-templates select="desc"/>
	<xsl:apply-templates select="remark"/>
	<xsl:call-template name="ctors"/>
	<xsl:call-template name="methods"/>
	<xsl:call-template name="apis"/>
	<xsl:call-template name="topic-end"/>
</BODY>
</HTML>
