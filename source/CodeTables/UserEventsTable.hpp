#ifndef USEREVENTTABLE_HPP
#define USEREVENTTABLE_HPP

namespace ProjectMaya {

	/**
	 * Maps int types to events emitted by the user
	 */
	enum UserEventTable {
		/// User requested program shutdown
		EXIT,

		/// Open scripting console
		CONSOLE_OPEN,

		/// Close scripting console
		CONSOLE_CLOSE
	};

}

#endif

