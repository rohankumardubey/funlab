/*
---            `funlab' 0.0.0 (c) 1978 by Marcin 'Amok' Konarski             ---

	main.cxx - this file is integral part of `funlab' project.

	i.  You may not make any changes in Copyright information.
	ii. You must attach Copyright information to any part of every copy
	    of this software.

Copyright:

 You are free to use this program as is, you can redistribute binary
 package freely but:
  1. You cannot use any part of sources of this software.
  2. You cannot redistribute any part of sources of this software.
  3. No reverse engineering is allowed.
  4. If you want redistribute binary package you cannot demand any fees
     for this software.
     You cannot even demand cost of the carrier (CD for example).
  5. You cannot include it to any commercial enterprise (for example 
     as a free add-on to payed software or payed newspaper).
 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE. Use it at your own risk.
*/

#include <libintl.h>
#include <unistd.h>
#include <yaal/yaal.hxx> /* all hAPI headers */
M_VCSID( "$Id: "__ID__" $" )

#include "version.hxx"
#include "setup.hxx"
#include "options.hxx"
#include "gui.hxx"
#include "hdetachedrenderer.hxx"
#include "hfunlab.hxx"

using namespace std;
using namespace yaal;
using namespace yaal::hcore;
using namespace yaal::hconsole;
using namespace yaal::tools;
using namespace yaal::tools::util;
using namespace funlab;

namespace funlab
{

OSetup setup;

}

int main ( int a_iArgc, char * a_ppcArgv [ ] )
	{
	M_PROLOG
/* variables declarations for main loop: */
	int l_iOpt = 0;
	int l_iCtr = 0, l_iPosition = 0;
	HString l_oArrow;
/* end. */
	try
		{
/* TO-DO: enter main loop code here */
		HSignalServiceFactory::get_instance();
		setup.f_pcProgramName = a_ppcArgv [ 0 ];
		l_iOpt = handle_program_options( a_iArgc, a_ppcArgv );
		hcore::log.rehash( setup.f_oLogPath, setup.f_pcProgramName );
		setup.test_setup();
/*		if ( ! console::is_enabled ( ) )enter_curses (); / * enabling ncurses ablilities*/
/* *BOOM* */
		if ( !! setup.f_oFormula )
			{
			HDetachedRenderer l_oRenderer;
			HFunlab* pf = NULL;
			HFunlab::ptr_t f( pf = new HFunlab( &l_oRenderer ) );
			l_iOpt = pf->push_formula( setup.f_oFormula ) ? 1 : 0;
			if ( l_iOpt )
				{
				l_iPosition = pf->error_position();
				for ( l_iCtr = 0; l_iCtr < l_iPosition; l_iCtr ++ )
					l_oArrow += '-';
				l_oArrow += 'v';
				::fprintf( stderr, _( "Formula syntax error ...\n%s at this place:\n%s\n%s\n" ),
						pf->error(), l_oArrow.raw(), setup.f_oFormula.raw() );
				}
			else
				{
				l_oRenderer.set_engine( f );
				l_oRenderer.render_surface();
				}
			}
		else
			l_iOpt = gui_start ( a_iArgc, a_ppcArgv );
		HConsole& cons = HCons::get_instance();
		if ( cons.is_enabled ( ) )
			cons.leave_curses (); /* ending ncurses sesion */
/* ... there is the place main loop ends. :OD-OT */
		}
	catch ( ... )
		{
		HConsole& cons = HCons::get_instance();
		if ( cons.is_enabled ( ) )
			cons.leave_curses (); /* ending ncurses sesion */
		throw;
		}
	fprintf ( stderr, "Done.\n" );
	return ( l_iOpt );
	M_FINAL
	}

