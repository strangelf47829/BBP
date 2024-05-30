#include "../include/errnoStrings.h"

// Strings
BBP::std::conststring strings[131] =
{
	"Operation not permitted", // EPERM
	"No such file or directory", // ENOENT
	"No such process", // ESRCH
	"Interrupted system call", //EINTR
	"Input/output error", //EIO
	"No such device or address", //ENXIO
	"Argument list too long", //E2BIG
	"Exec format error", //ENOEXEC
	"Bad file descriptor", //EBADF
	"No child processes", //ECHILD
	"Resource temporarily unavailable", // EAGAIN
	"Cannot allocate memory", // ENOMEM
	"Permission denied", // EACCESS
	"Bad address", // EFAULT
	"Block device required", //ENOTBLK
	"Device or resource busy", //EBUSY
	"File exists", //EEXIST
	"Invalid cross-device link", //EXDEV
	"No such device", //ENODEV
	"Not a directory", //ENOTDIR
	"Is a directory", //EISDIR
	"Invalid argument", //EINVAL
	"Too many open files in system", // ENFILE
	"Too many open files", //EMFILE
	"Innappropriate ioctl for device", //ENOTTY
	"Text file busy", //ETXTBUSY
	"File too large", //EFBIG
	"No space left on device", //EFBIG
	"Illegal seek", //ESPIPE
	"Read-only file system", //EROFS
	"Too many links", // EMLINK
	"Broken pipe", // EPIPE
	"Numerical argument out of domain", // EDOM
	"Numerical result out of range", // ERANGE
	"Resource deadlock avoided", //EDEADLK
	"File name too long", // ENAMETOOLONG
	"No locks available", // ENOLCK
	"Function not implemented", // ENOSYS
	"Directory not empty", // ENOTEMPTY
	"Too many levels of symbolic links", // ELOOP
	"Resource temporarily unavailable", // EWOULDBLOCK
	"No message of desired type", // ENOMSG
	"Identifier removed", // EIDRM
	"Channel number out of range", // ECHRNG
	"Level 2 not synchronised", // EL2NSYNC
	"Level 3 halted", // EL3HLT
	"Level 3 reset", // EL3RST
	"Link number out of range", // ELNRNG
	"Protocol driver not attached", // EUNATCH
	"No CSI structure available", // ENOCSI
	"Level 2 halted", // EL2HLT
	"Invalid exchange", // EBADE
	"Invalid request descriptor", // EBADR
	"Exchange full", // EXFULL
	"No anode", //ENOANO
	"Invalid request code", //EBADRQC
	"Invalid slot", // EBADSLT
	"Resource deadlock avoided", // EBFONT
	"Device not a stream", //ENOSTR
	"No data available", //ENODATA
	"Timer expired", // ETIME
	"Out of stream resources", //ENOSR
	"Machine is not on the network", // ENONET
	"Package is not installed", // ENOPKG
	"Object is remote", // EREMOTE
	"Link has been severed", // ENOLINK
	"Advertise error", // EADV
	"Srmount error", // ESRMNT
	"Communication error on send", // ECOMM
	"Protocol error", //EPROTO
	"Multihop attempted", //EMULTIHOP
	"RFS Specific error", //EDOTDOT
	"Bad message", //EBADMSG
	"Value too large for defined data type", // EOVERFLOW
	"Name not unique on network", //ENOTUNIQ
	"File descriptor in bad state", //EBADFD
	"Remote address changed", // EREMCHG
	"Can not access a needed shared library", //ELIBACC
	"Accessing a corrupted shared library", // ELIBBAD
	".lib section in a.out corrupted", //ELIBSCN
	"Attempting to link in too many shared libraries", //ELIBMAX
	"Cannot exec a shared library directly", // ELIBEXEC
	"Invalid or incomplete multibyte or wide character", //EILSEQ
	"Interrupted system call should be restarted", //ERESTART
	"Streams pipe error", // ESTRPIPE
	"Too many users", // EUSERS
	"Socket operation on non-socket", // ENOTSOCK
	"Destination address required", // EDESTADDRREQ
	"Message too long", // EMSGSIZE
	"Protocol wrong type for socket", //EPROTOTYPE
	"Protocol not available", // ENOPTOTOOPT
	"Protocol not supported", // ENOPROTONOSUPPORT
	"Socket type not supported", // ESOCKNOSUPPORT
	"Operation not supported", // EOPNOTSUPP
	"Protocol family not supported", //EPFNOSUPPORT
	"Address family not supported by protocol", //EAFNOSUPPORT
	"Address already in use", //ADDRINUSE
	"Cannot assign requested address", //EADDRNOTAVAIL
	"Network is down", //ENETDOWN
	"Network is unreachable", //ENETUNREACH
	"Network dropped connection on reset", //ENETRESET
	"Software caused connection abort", //ECONNABORTED
	"Connection reset by peer", //ENOBUFS
	"Transport endpoint is already connected", //EISCONN
	"Transpoint endpoint is not connected", //ENOTCONN
	"Cannot send after transport endpoint shutdown", // ESHUTDOWN
	"Too many references: cannot splice", //ETOOMANYREFS
	"Connection timed out", // ETIMEDOUT
	"Connection refused", //ECONNREFUSED
	"Host is down", //EHOSTDOWN
	"No route to host", // EHOSTUNREACH
	"Operation already in progress", // EINPROGRESS
	"Stale file handle", // ESTALE
	"Structure needs cleaning", //EUCLEAN
	"Not a XENIX named type file", //ENOTNAM
	"No XENIX semaphores available", //ENAVAIL
	"Is a named type file", // EISNAM
	"Remote I/O error", //EREMOTEIO
	"Disk quota exceeded", //EDQUOT
	"No medium found", // ENOMEDIUM
	"Wrong medium type", // EMEDIUMTYPE
	"Operation cancelled", // ECANCELED
	"Required key not available", // ENOKEY
	"Key has expired", // EKEYEXPIRED
	"Key has been revoked", //EKEYREVOKED
	"Key was rejected by service", //EKEYREJECTED
	"Owner died", // EOWNDEAD
	"State not recoverable", //ENOTRECOVERABLE
	"Operation not possible due to RF-kill", //ERFKILL
	"Memory page has hardware error", // EHWPOISON
	"Operation not supported", // ENOTSUP
};

// The indicies thereof
BBP::std::errno_t indicies[] =
{
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	17,
	19,
	19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	33,
	34,
	35,
	36,
	37,
	38,
	39,
	40,
	11,
	42,
	43,
	44,
	45,
	46,
	47,
	48,
	49,
	50,
	51,
	52,
	53,
	54,
	55,
	56,
	57,
	35,
	59,
	60,
	61,
	62,
	63,
	64,
	65,
	66,
	67,
	68,
	69,
	70,
	71,
	72,
	73,
	74,
	75,
	76,
	77,
	78,
	79,
	80,
	81,
	82,
	83,
	84,
	85,
	68,
	87,
	88,
	89,
	90,
	81,
	92,
	93,
	94,
	95,
	96,
	97,
	98,
	99,
	100,
	101,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,
	112,
	113,
	114,
	115,
	116,
	117,
	118,
	119,
	120,
	121,
	122,
	123,
	124,
	125,
	126,
	127,
	128,
	129,
	130,
	131,
	132,
	133,
	95
};

void BBP::std::strerror(std::errno_t error, std::string &str)
{
	constexpr std::size_t indexCount = sizeof(indicies) / sizeof(std::index_t);

	// Loop over indicies
	for (std::index_t idx = 0; idx < indexCount; idx++)
	{
		// If error fits in, do that
		if (indicies[idx] == error)
		{
			str = std::String(strings[indicies[idx]]);
			return;
		}
	}

	// Nothing found, set error
	strerror(EOPNOTSUPP, str);
}