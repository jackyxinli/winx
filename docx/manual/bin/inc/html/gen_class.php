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

function show_ctors($sentences, $path)
{
	foreach ($sentences as $s)
	{
		if (isset($s->comment))
			$ctordoc = $s->comment;
		else if (isset($s->ctor))
		{
			$ctors[] = $ctordoc;
			$ctors[] = $s->ctor;
		}
	}
	
	$count = count($ctors);
	if ($count == 0)
		return;
	
	echo "<H4>¹¹Ôìº¯Êý</H4>\n\t<TABLE height=\"1\">
		<TR VALIGN=\"top\">
			<TH align=\"left\" width=\"35%\" height=\"19\">Constructor</TH>
			<TH align=\"left\" width=\"65%\" height=\"19\">Description</TH>
		</TR>\n";
		
	for ($i = 0; $i < $count; $i += 2)
	{
		echo "<TR VALIGN=\"top\"><TD width=\"35%\" height=\"19\"><b><a>";
		
						<xsl:attribute name="href">
							<xsl:value-of select="/ksdn/doc_path"/>
							<xsl:text>_</xsl:text>
							<xsl:value-of select="../class_decl/class_name"/>
							<xsl:if test="ctor_args">(<xsl:value-of select="ctor_args"/>)</xsl:if>
							<xsl:text>.htm</xsl:text>
						</xsl:attribute>
						<xsl:value-of select="../class_decl/class_name"/>
						<xsl:if test="ctor_args">(<xsl:value-of select="ctor_args"/>)</xsl:if>
					</a>
				</b>
			</TD>
			<TD width="65%" height="19">
				<xsl:copy-of select="brief"/>
			</TD>
		</TR>
	}
	echo "</TABLE>\n";
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
