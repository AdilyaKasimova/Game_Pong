/**
 * \brief Structure representing the state of a button.
 */
struct Button_State {
    bool is_down; /**< Indicates whether the button is currently pressed. */
    bool changed; /**< Indicates whether the state of the button has changed since the last frame. */
};


/**
 * \brief Enumeration of button identifiers.
 */
enum {
    BUTTON_UP, /**< Identifier for the up button. */
    BUTTON_DOWN, /**< Identifier for the down button. */
    BUTTON_W, /**< Identifier for the W button. */
    BUTTON_S, /**< Identifier for the S button. */
    BUTTON_LEFT, /**< Identifier for the left arrow button. */
    BUTTON_RIGHT, /**< Identifier for the right arrow button. */
    BUTTON_ENTER, /**< Identifier for the enter button. */
    BUTTON_ESCAPE, /**< Identifier for the escape button. */

    BUTTON_COUNT, /**< Total number of buttons. */
};

/**
 * \brief Structure representing the input state.
 */
struct Input {
    Button_State buttons[BUTTON_COUNT]; /**< Array of button states. */
};