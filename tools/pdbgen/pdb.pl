# The GIMP -- an image manipulation program
# Copyright (C) 1998-2003 Manish Singh <yosh@gimp.org>

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

package Gimp::CodeGen::pdb;

%arg_types = (
    int32  => { name           => 'INT32',
		type           => 'gint32 ',
		init_value     => '0',
		get_value_func => '$var = g_value_get_int ($value)',
		set_value_func => 'g_value_set_int ($value, $var)' },
    int16  => { name           => 'INT16',
		type           => 'gint16 ',
		init_value     => '0',
		get_value_func => '$var = g_value_get_int ($value)',
		set_value_func => 'g_value_set_int ($value, $var)' },
    int8   => { name           => 'INT8' ,
		type           => 'guint8 ',
		init_value     => '0',
		get_value_func => '$var = g_value_get_int ($value)',
		set_value_func => 'g_value_set_int ($value, $var)' },

    float  => { name           => 'FLOAT',
		type           => 'gdouble ',
		init_value     => '0.0',
		get_value_func => '$var = g_value_get_double ($value)',
		set_value_func => 'g_value_set_double ($value, $var)' },
    string => { name           => 'STRING',
		type           => 'gchar *',
		init_value     => 'NULL',
		get_value_func => '$var = (gchar *) g_value_get_string ($value)',
		set_value_func => 'g_value_take_string ($value, $var)' },

    int32array  => { name           => 'INT32ARRAY',
		     type           => 'gint32 *',
		     array          => 1,
		     init_value     => 'NULL',
		     get_value_func => '$var = g_value_get_pointer ($value)',
		     set_value_func => 'g_value_set_pointer ($value, $var)' },
    int16array  => { name           => 'INT16ARRAY',
		     type           => 'gint16 *',
		     array          => 1,
		     init_value     => 'NULL',
		     get_value_func => '$var = g_value_get_pointer ($value)',
		     set_value_func => 'g_value_set_pointer ($value, $var)' },
    int8array   => { name           => 'INT8ARRAY',
		     type           => 'guint8 *',
		     array          => 1,
		     init_value     => 'NULL',
		     get_value_func => '$var = g_value_get_pointer ($value)',
		     set_value_func => 'g_value_set_pointer ($value, $var)' },
    floatarray  => { name           => 'FLOATARRAY',
		     type           => 'gdouble *',
		     array          => 1,
		     init_value     => 'NULL',
		     get_value_func => '$var = g_value_get_pointer ($value)',
		     set_value_func => 'g_value_set_pointer ($value, $var)' },
    stringarray => { name           => 'STRINGARRAY',
		     type           => 'gchar **',
		     array          => 1,
		     init_value     => 'NULL',
		     get_value_func => '$var = g_value_get_pointer ($value)',
		     set_value_func => 'g_value_set_pointer ($value, $var)' },

    color => { name           => 'COLOR' , 
	       type           => 'GimpRGB ',
	       struct         => 1,
	       init_value     => '{ 0.0, 0.0, 0.0, 1.0 }',
	       get_value_func => 'gimp_value_get_rgb ($value, &$var)',
	       set_value_func => 'gimp_value_set_rgb ($value, &$var)',
	       headers        => [ qw("libgimpcolor/gimpcolor.h") ] },

    display    => { name           => 'DISPLAY',
		    type           => 'GimpObject *',
		    id             => 1,
		    init_value     => 'NULL',
		    get_value_func => '$var = gimp_value_get_display ($value, gimp)',
		    set_value_func => 'gimp_value_set_display ($value, $var)' },
    image      => { name           => 'IMAGE',
		    type           => 'GimpImage *', 
		    id             => 1,
		    init_value     => 'NULL',
		    get_value_func => '$var = gimp_value_get_image ($value, gimp)',
		    set_value_func => 'gimp_value_set_image ($value, $var)',
		    headers        => [ qw("core/gimpimage.h") ] },
    layer      => { name           => 'LAYER',
		    type           => 'GimpLayer *', 
		    id             => 1,
		    init_value     => 'NULL',
		    get_value_func => '$var = (GimpLayer *) gimp_value_get_item ($value, gimp, GIMP_TYPE_LAYER)',
		    set_value_func => 'gimp_value_set_item ($value, GIMP_ITEM ($var))',
		    headers        => [ qw("core/gimplayer.h") ] },
    channel    => { name           => 'CHANNEL',
		    type           => 'GimpChannel *',
		    id             => 1,
		    init_value     => 'NULL',
		    get_value_func => '$var = (GimpChannel *) gimp_value_get_item ($value, gimp, GIMP_TYPE_CHANNEL)',
		    set_value_func => 'gimp_value_set_item ($value, GIMP_ITEM ($var))',
		    headers        => [ qw("core/gimpchannel.h") ] },
    drawable   => { name           => 'DRAWABLE',
		    type           => 'GimpDrawable *',
		    id             => 1,
		    init_value     => 'NULL',
		    get_value_func => '$var = (GimpDrawable *) gimp_value_get_item ($value, gimp, GIMP_TYPE_DRAWABLE)',
		    set_value_func => 'gimp_value_set_item ($value, GIMP_ITEM ($var))',
		    headers        => [ qw("core/gimpdrawable.h") ] },
    selection  => { name           => 'SELECTION',
		    type           => 'GimpChannel *',
		    id             => 1,
		    init_value     => 'NULL',
		    get_value_func => '$var = (GimpChannel *) gimp_value_get_item ($value, gimp, GIMP_TYPE_CHANNEL)',
		    set_value_func => 'gimp_value_set_item ($value, GIMP_ITEM ($var))',
		    headers        => [ qw("core/gimpchannel.h") ] },
    layer_mask => { name           => 'CHANNEL',
		    type           => 'GimpLayerMask *', 
		    id             => 1,
		    init_value     => 'NULL',
		    get_value_func => '$var = (GimpLayerMask *) gimp_value_get_item ($value, gimp, GIMP_TYPE_LAYER_MASK)',
		    set_value_func => 'gimp_value_set_item ($value, GIMP_ITEM ($var))',
		    headers        => [ qw("core/gimplayermask.h") ] },
    vectors    => { name           => 'VECTORS',
		    type           => 'GimpVectors *', 
		    id             => 1,
		    init_value     => 'NULL',
		    get_value_func => '$var = (GimpVectors *) gimp_value_get_item ($value, gimp, GIMP_TYPE_VECTORS)',
		    set_value_func => 'gimp_value_set_item ($value, GIMP_ITEM ($var))',
		    headers        => [ qw("vectors/gimpvectors.h") ] },

    parasite => { name           => 'PARASITE',
		  type           => 'GimpParasite *',
		  init_value     => 'NULL',
		  get_value_func => '$var = g_value_get_boxed ($value)',
		  set_value_func => 'g_value_take_boxed ($value, $var)',
		  headers => [ qw("libgimpbase/gimpbase.h") ] },

    boundary => { name => 'BOUNDARY', type => 'gpointer ' }, # ??? FIXME
    status   => { name => 'STATUS'  , type => 'gpointer ' }, # ??? FIXME

    # Special cases
    enum    => { name           => 'INT32',
		 type           => 'gint32 ',
		 init_value     => '0',
		 get_value_func => '$var = g_value_get_enum ($value)',
		 set_value_func => 'g_value_set_enum ($value, $var)' },
    boolean => { name           => 'INT32',
		 type           => 'gboolean ',
		 init_value     => 'FALSE',
		 get_value_func => '$var = g_value_get_boolean ($value)',
		 set_value_func => 'g_value_set_boolean ($value, $var)' },
    tattoo  => { name           => 'INT32',
		 type           => 'gint32 ',
		 init_value     => '0',
		 get_value_func => '$var = g_value_get_uint ($value)',
		 set_value_func => 'g_value_set_uint ($value, $var)' },
    guide   => { name           => 'INT32',
		 type           => 'gint32 ',
		 id             => 1,
		 init_value     => '0',
		 get_value_func => '$var = g_value_get_uint ($value)',
		 set_value_func => 'g_value_set_uint ($value, $var)' },
    unit    => { name           => 'INT32',
		 type           => 'GimpUnit ',
		 init_value     => '0',
		 get_value_func => '$var = g_value_get_int ($value)',
		 set_value_func => 'g_value_set_int ($value, $var)' },

    region => { name => 'REGION', type => 'gpointer ' } # not supported
);

# Split out the parts of an arg constraint
sub arg_parse {
    my %premap = (
	'<'  => '<=',
	'>'  => '>=',
	'<=' => '<',
	'>=' => '>'
    );

    my %postmap = (
	'<'  => '>=',
	'>'  => '<=',
	'<=' => '>',
	'>=' => '<'
    );

    my $arg = shift;

    if ($arg =~ /^enum (\w+)(.*)/) {
	my ($name, $remove) = ($1, $2);
	my @retvals = ('enum', $name);

	if ($remove && $remove =~ m@ \(no @) {
	    chop $remove; ($remove = substr($remove, 5)) =~ s/ $//;
	    push @retvals, split(/,\s*/, $remove);
	}

	return @retvals;
    }
    elsif ($arg =~ /^unit(?: \(min (.*?)\))?/) {
	my @retvals = ('unit');
	push @retvals, $1 if $1;
	return @retvals;
    }
    elsif ($arg =~ /^(?:([+-.\d][^\s]*) \s* (<=|<))?
		     \s* (\w+) \s*
		     (?:(<=|<) \s* ([+-.\d][^\s]*))?
		   /x) {
	return ($3, $1, $2 ? $premap{$2} : $2, $5, $4 ? $postmap{$4} : $4);
    }
}

1;
