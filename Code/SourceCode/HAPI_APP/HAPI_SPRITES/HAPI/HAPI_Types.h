#pragma once
/*
	Aliases, enums and structs used with HAPI
*/

#include <string>
#include <sstream>
#include <vector>

namespace HAPISPACE {

	/** <summary> an alias for a type used to hold a byte (1 byte or 8 bits) of data </summary> */
	using BYTE = unsigned char;

	/** <summary> an alias for a type used to hold an unsigned WORD (2 bytes or 16 bits) of data </summary> */
	using WORD = unsigned short;

	/** <summary> an alias for a type used to hold a signed WORD (2 bytes or 16 bits) of data </summary> */
	using SWORD = signed short;

	/** <summary>	an alias for a type used to hold two WORDS (4 bytes or 32 bits) of data. </summary> */
	using DWORD = unsigned long;

	/** <summary>	Flag values that can be used during initialise. </summary> */
	enum HAPI_InitFlags
	{
		eDefaults = 0,
		eFullscreen = 1 << 0,
		eReserved1 = 1 << 1,
		eReserved2 = 1 << 2
	};

	/**
	 * <summary>
	 * Text format bit flags that can be combined e.g. HAPISPACE::eBold & HAPISPACE::eItalic.
	 * </summary>
	 */
	enum HAPI_TextStyle
	{
		eRegular = 0,			// Regular characters, no style
		eBold = 1 << 0,			// Bold characters
		eItalic = 1 << 1,		// Italic characters NOTE: not supported from 2.35
		eUnderlined = 1 << 2,	// Underlined characters
		eStrikeThrough = 1 << 3	// Strike through characters
	};

	/** <summary>	HAPI Error codes. </summary> */
	enum class HAPI_ErrorCodes
	{
		H_OK = 0,
		H_INITIALISATION_ERROR,
		H_INVALID_PARAMETER,
		H_OUT_OF_MEMORY,
		H_FILE_NOT_FOUND,
		H_INTERNAL_ERROR,
		H_UNSUPPORTED,
		H_DEVICE_NOT_CONNECTED,
		H_USER_REPORTED_ERROR,
		H_NUM_HAPI_ERROR_CODES
	};

	/** <summary>	HAPI systems as specified in errors. </summary> */
	enum class HAPI_System
	{
		H_CORE_SYSTEM = 0,
		H_OS_SYSTEM,
		H_RENDERER_SYSTEM,
		H_SOUND_SYSTEM,
		H_INPUT_SYSTEM,
		H_INTERNAL_SYSTEM,
		H_SPRITES_SYSTEM,	
		H_XML_SYSTEM,
		H_NUM_HAPI_SYSTEMS
	};

	/** <summary>	Message box button types for use in the HAPI_MessageBox call. </summary> */
	enum class HAPI_ButtonType
	{
		eButtonTypeOk,				// Just an OK button
		eButtonTypeOkCancel,		// OK and CANCEL buttons
		eButtonTypeYesNo,			// YES and NO buttons
		eButtonTypeYesNoCancel		// YES, NO and CANCEL buttons
	};

	/** <summary>	Message box return types from the HAPI_UserMessage call and file dialog call. </summary> */
	enum class HAPI_UserResponse
	{
		eUserResponseUnknown = -1,
		eUserResponseOk,		// OK button clicked by user
		eUserResponseCancel,	// CANCEL button clicked
		eUserResponseYes,		// YES button clicked
		eUserResponseNo			// NO button clicked
	};

	/**
	* \struct HAPI_TMouseData
	* \brief Contains information about the current mouse state
	*/
	struct HAPI_TMouseData final
	{
		int x{ 0 };
		int y{ 0 };
		int wheelMovement{ 0 };
		bool leftButtonDown{ false };
		bool rightButtonDown{ false };
		bool middleButtonDown{ false };
		bool xButton1Down{ false };
		bool xButton2Down{ false };
	};

	/**
	* \struct HAPI_TKeyboardData
	* \brief Contains information about the current keyboard state, true if key down false if not
	* Note: you can use the defines in HAPI_InputCodes.h as indices into the  scanCode array
	*/
	struct HAPI_TKeyboardData final
	{
		bool scanCode[256]{};
	};

	// The maximum number of buttons a controller may have
	// Note: use this rather than hard coding the number as it may change in the future
	constexpr int HAPI_MAX_BUTTON_ARRAY_SIZE{ 16 };

	/**
	* \struct HAPI_TControllerData
	* \brief Contains information about a controller state
	* Note that not all controllers have HAPI_MAX_BUTTON_ARRAY_SIZE of each type of button
	* Note: you can use the defines in HAPI_InputCodes.h as indices into the arrays
	*/
	struct HAPI_TControllerData final
	{
		bool isAttached{ false }; // only if this is true are the values below valid
		bool digitalButtons[HAPI_MAX_BUTTON_ARRAY_SIZE]{};	// On or off
		int analogueButtons[HAPI_MAX_BUTTON_ARRAY_SIZE]{};	// Value range is -32768 to +32767		
	};

	/** <summary>	Used by HAPI to represent a colour. Values range from 0 to 255 per channel. </summary> */
	// Note: in use it is best to pass this by value not const reference due to its small size
	struct HAPI_TColour final
	{
		BYTE red{ 255 };
		BYTE green{ 255 };
		BYTE blue{ 255 };
		BYTE alpha{ 255 };

		// Constructor 1 - Colour defaults to white	
		HAPI_TColour() noexcept = default;

		// Constructor 2 - Colour channels passed in, alpha defaults to 255 (opaque) if not provided
		HAPI_TColour(BYTE r, BYTE g, BYTE b, BYTE a = 255) noexcept : red(r), green(g), blue(b), alpha(a) {}

		// Constructor 3 - gray
		HAPI_TColour(BYTE gray, BYTE a = 255) noexcept : red(gray), green(gray), blue(gray), alpha(a) {}

		// Helpers
		static HAPI_TColour BLACK;
		static HAPI_TColour WHITE;
		static HAPI_TColour RED;
		static HAPI_TColour GREEN;
		static HAPI_TColour BLUE;
		static HAPI_TColour YELLOW;
		static HAPI_TColour CYAN;
		static HAPI_TColour MAGENTA;
		static HAPI_TColour HORRID_PINK;
		static HAPI_TColour ZERO; // 0,0,0,0

		// Returns a random colour
		static HAPI_TColour Random() { return HAPI_TColour(rand() % 255, rand() % 255, rand() % 255); }

		friend std::ostream& operator<<(std::ostream& os, const HAPI_TColour& col);

		std::string ToString() const { std::stringstream str; str << *this; return str.str(); }

		// Modulate. Note: must be accurate so no bit shift used
		HAPI_TColour& operator*=(const HAPI_TColour &rhs)
		{
			red = (red * rhs.red) / 255;
			green = (green * rhs.green) / 255;
			blue = (blue * rhs.blue) / 255;
			alpha = (alpha * rhs.alpha) / 255;
			return *this;
		}
	};

	// Can be used with cout to output colour to output pane and log
	inline std::ostream& operator<<(std::ostream& os, const HAPI_TColour& col)
	{
		os << "R: " + std::to_string(col.red) +
			" G: " + std::to_string(col.green) +
			" B: " + std::to_string(col.blue) +
			" A: " + std::to_string(col.alpha);
		return os;
	}

	// Equivelance operator
	inline bool operator==(const HAPI_TColour& lhs, const HAPI_TColour& rhs)
	{		
		return (*(DWORD*)&lhs == *(DWORD*)&rhs);		
	}

	inline bool operator!=(const HAPI_TColour& lhs, const HAPI_TColour& rhs)
	{
		return !(lhs == rhs);
	}

	// Subtract one colour from the other handling wrap around
	inline HAPI_TColour operator- (const HAPI_TColour& lhs, const HAPI_TColour& rhs)
	{
		// Allow loop around
		int r = lhs.red - rhs.red;
		if (r < 0) r += 255;
		int g= lhs.green - rhs.green;
		if (g < 0) g += 255;
		int b = lhs.blue - rhs.blue;
		if (b < 0) b += 255;
		int a = lhs.alpha - rhs.alpha;	
		if (a < 0) a += 255;
		return HAPI_TColour((BYTE)r, (BYTE)g, (BYTE)b, (BYTE)a);
	}

	// Modulate one colour by another by multiplying channels
	inline HAPI_TColour operator* (const HAPI_TColour& lhs, HAPI_TColour rhs)
	{
		HAPI_TColour sourceMod{ lhs };
		sourceMod.red = (lhs.red * rhs.red) >> 8;
		sourceMod.green = (lhs.green * rhs.green) >> 8;
		sourceMod.blue = (lhs.blue * rhs.blue) >> 8;
		sourceMod.alpha = (lhs.alpha * rhs.alpha) >> 8;
		return sourceMod;
	}

	/**
	 * <summary>
	 * used by HAPI to describe a sound's data make up Note: all HAPI loaded sounds have a bit depth
	 * of 16 (two bytes or a SWORD)
	 * </summary>
	 */
	struct HAPI_TSoundFormat final
	{
		unsigned int numChannels{ 0 };	// 1 is mono, 2 is stereo etc.
		unsigned int numSamples{ 0 };	// How many samples in total
		unsigned int sampleRate{ 0 };	// How many samples per second at normal play back speed 44.1K = CD Quality
		// 'Length in seconds' helper calculation:
		float Seconds() const { return (numChannels * numSamples) / (float)sampleRate; }
	};

	/** <summary>	Structure used to set sound playback options. </summary> */
	struct HAPI_TSoundOptions final
	{
		float pan{ 0 };				// Left is -1 right is 1. Note: if you provide a 3D position, this is ignored
		float volume{ 1.0f };		// from 0 - 1 (full)
		float pitch{ 1.0f };		// 2 would double the pitch and make it play in half the time
		
		float pos3Dx{ 0 };			// If any of these 3 are set to !=0 then these are used to position the sound 
		float pos3Dy{ 0 };			// and pan is ignored
		float pos3Dz{ 0 };			//	Note: 3D positioning only works for mono sounds

		bool loop{ false };			// Should the sound loop when it gets to the end

		// Constructor with defaults with most common settings first
		HAPI_TSoundOptions(float volumeI = 1.0f, bool loopI = false, float panI = 0, float pitchI = 1.0f) noexcept :
			volume(volumeI), loop(loopI), pan(panI), pitch(pitchI) {}
	};

	/** <summary>	Values that represent the media status. </summary> */
	enum class HAPI_MediaStatus
	{
		eError,		// An error occurred		
		eStarting,	// About to start
		eStopped,	// Stopped and sitting at start / first frame
		ePaused,	// Paused and sitting at a frame
		ePlaying	// Playing
	};	

	/** <summary>	Structure used to return information about media. </summary> */
	struct HAPI_TStreamedMediaInfo final
	{
		HAPI_MediaStatus status{ HAPI_MediaStatus::eError }; // See HAPI_StreamedMediaStatus enum
		//unsigned int numAudioStreams{ 0 };	// Normally 1 but some Video can have multiple audi streams
		unsigned int audioChannels{ 0 }; // 1 for mono, 2 for stereo
		unsigned int audioSamplingRate{ 0 }; // Samples per second. E.g. 44100 is CD format quality
		bool loop{ false };	// Looping or not
		float duration{ 0 }; // Total duration in seconds
		float currentPosition{ 0 };	// Current playback position in seconds

		// From version 2.5 movie playback is not enabled. It may be added back in later if enough people want it.
		//unsigned int numVideoStreams{ 0 };  // Normally 1
		//unsigned int numSubtitleStreams{ 0 }; // Normally 0 but some Video can subtitles
		//float frameRate{ 0 };	// Video frames per second
	};

	// Used to specify filters when calling FileDialog
	struct HAPI_TFileDialogFilters final
	{
		std::string filterName;	// e.g. Bitmap Files
		std::string filter;		// e.g. *.bmp
	};

	// Types of XML node
	enum class HAPI_XMLNodeType
	{
		eNull,
		eDocument,
		eElement,
		ePcdata,
		eCdata,
		eComment,
		ePi,
		eDeclaration
	};

	// XML attribute
	class CHapiXMLAttribute
	{
	private:
		std::string m_name;
		std::string m_stringValue;
	public:
		CHapiXMLAttribute(const std::string &name, const std::string &stringValue) noexcept : m_name(name), m_stringValue(stringValue) {}
		CHapiXMLAttribute(const std::string &name, const char cvalue[]) noexcept : m_name(name), m_stringValue(cvalue)  {}
		template <typename T>		
		CHapiXMLAttribute(const std::string &name, const T &value) noexcept : m_name(name), m_stringValue(std::to_string(value)) {}
		CHapiXMLAttribute() = default;

		// Retrieve the value as a string
		std::string AsString() const { return m_stringValue; }

		// Retrieve the value as an int
		int AsInt() const { return stoi(m_stringValue); }

		// Retrieve as bool
		bool AsBool() const { return (AsInt() == 1); }

		// Retrieve the value as a float
		float AsFloat() const { return stof(m_stringValue); }

		// Retrieve the value as a double
		double AsDouble() const { return stod(m_stringValue); }

		// Needed for when working with templated values
		void As(int &i) { i=AsInt(); }
		void As(bool &i) { i = AsBool(); }
		void As(float &i) { i = AsFloat(); }
		void As(double &i) { i = AsDouble(); }
		void As(std::string &i) { i = AsString(); }

		// Retrieve the name of this value (if provided)
		const std::string& GetName() const { return m_name; }
	};

	// An XML node 
	class CHapiXMLNode
	{
	private:
		std::string m_name;
		HAPI_XMLNodeType m_type;
		std::string m_value;
		std::vector<CHapiXMLAttribute> m_attributes;
		std::vector<CHapiXMLNode*> m_children;
		CHapiXMLNode *m_parent{ nullptr };
	public:
		CHapiXMLNode(const std::string& name, HAPI_XMLNodeType type = HAPI_XMLNodeType::eElement, std::string value = std::string(),
			CHapiXMLNode *parent=nullptr) noexcept : m_name(name), m_type(type), m_value(value), m_parent(parent) {}

		// Add an attribute to this node
		void AddAttribute(CHapiXMLAttribute attribute) {m_attributes.push_back(std::move(attribute));}

		// Add a child node to this node
		void AddChild(CHapiXMLNode* child) { child->m_parent = this; m_children.push_back(child); }

		// Get this nodes name
		const std::string& GetName() const { return m_name; }

		// Get this nodes tyype
		HAPI_XMLNodeType GetType() const { return m_type; }
		
		// Changed 1.20 so the node's value can also be accessed as if it were an attribute
		CHapiXMLAttribute GetValue() const { return CHapiXMLAttribute(m_name, m_value); }

		// Get all the attributes for this node
		const std::vector<CHapiXMLAttribute>& GetAttributes() const { return m_attributes; }

		// Get all child nodes of this node
		const std::vector<CHapiXMLNode*>& GetChildren() const { return m_children; }

		// Get the parent node of this node
		CHapiXMLNode * GetParent() const { return m_parent; }

		//  Returns false if does not exist
		bool GetAttributeWithName(const std::string &name, CHapiXMLAttribute& attribute, bool reportNotFound = true) const;

		template <typename T>
		bool FillFromAttribute(const std::string &name, T& value, bool reportNotFound = true)
		{
			CHapiXMLAttribute attribute;
			if (!GetAttributeWithName(name, attribute, reportNotFound))
				return false;
			attribute.As(value);
			return true;
		}

		// Helpers
		static std::string ColourToString(HAPI_TColour col);
		static HAPI_TColour StringToColour(const std::string &str);

	};

	class CXMLPimpl;
	
	// Used for working with XML files
	class CHapiXML
	{
	private:
		CXMLPimpl *m_pimpl{ nullptr };
	public:
		// Constructor that loads XML file
		CHapiXML(const std::string &filename);

		// Constructor that creates an XML tree
		CHapiXML(CHapiXMLNode *rootNode);

		// Construct from an XML byte stream (UTF-8)
		CHapiXML(const std::vector<BYTE>& byteStream);

		~CHapiXML();

		// Used to determine if we have loaded ok
		bool HasData() const;

		// Saves tree as a new XML file, returns false on error
		bool Save(const std::string &filename);

		// Outputs the loaded XML file to the Output pane showing all nodes and attributes
		void DebugOutputStructure() const;

		// Finds and returns the first node in the tree called 'name'. Returns nullptr if it does not exist.
		// If startNode is nullptr starts at top of tree
		CHapiXMLNode* GetFirstNodeWithName(const std::string& name, CHapiXMLNode* startNode=nullptr, bool reportNotFound=true) const;

		// Returns a vector of all nodes called 'name' and their attributes
		//  If startNode is nullptr starts at top of tree
		std::vector<CHapiXMLNode*> GetAllNodesWithName(const std::string& name, CHapiXMLNode* startNode = nullptr) const;
		
		// Returns the root of the tree
		CHapiXMLNode* GetRootNode() const;

		// Converts to byte stream
		std::vector<BYTE> GetByteStream() const;

		// Helper functions
		static HAPI_TColour StringToColour(const std::string &str);
		static std::string ColourToString(HAPI_TColour col);
	};
}