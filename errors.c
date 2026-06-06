#include "errors.h"

const char *err_to_str(int err)
{
	switch (err)
	{
	case ERR_INVALID_FORMAT:
		return "The format of the report is invalid.";

	case ERR_SURNAME_NOT_SPECIFIED:
		return "Surname not specified.";

	case ERR_NAME_NOT_SPECIFIED:
		return "Name not specified.";

	case ERR_AGE_NOT_SPECIFIED:
		return "Age not specified.";

	case ERR_RACE_NOT_SPECIFIED:
		return "Race not specified.";

	case ERR_CITY_NOT_SPECIFIED:
		return "City not specified.";

	case ERR_FELONY_AGE_NOT_SPECIFIED:
		return "Felony age not specified.";

	case ERR_INVALID_AGE:
		return "Invalid age.";

	case ERR_INVALID_FELONY_AGE:
		return "Invalid felony age.";

	case ERR_INVALID_EDUCATION:
		return "Invalid education value.";

	case ERR_INVALID_M_VICTIMS:
		return "Invalid male victims value.";

	case ERR_INVALID_F_VICTIMS:
		return "Invalid female victims value.";

	case ERR_INVALID_NUM_VICTIMS:
		return "Invalid number of victims value.";

	case ERR_INVALID_TOTAL_VICTIMS:
		return "Mismatch in total victims.";

	case ERR_FILE_NOT_FOUND:
		return "Could not find or open file";
	case RESULT_NOTHING_TO_SAVE:
		return "Nothing to save";
	case RESULT_NO_ENTRIES:
		return "List is empty";

	case ERR_INVALID_COMMAND:
		return "Invalid command";

	case ERR_INVALID_ARGS:
		return "Command arguments are invalid or not present";

	case ERR_MATCHES_NOT_FOUND:
		return "No matches found";

	case ERR_INDEX_NOT_FOUND:
		return "Could not find element in list";

	case ERR_COMMAND_NOT_FOUND:
		return "Command not found";

	default:
		return "Unknown error.";
	}
}
