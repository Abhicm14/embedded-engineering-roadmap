/**
 * @file command_parser.c
 * @brief Lightweight, non-blocking serial CLI command tokenizer and dispatcher.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef void (*CommandHandler_t)(int argc, char *argv[]);

typedef struct {
    const char       *name;
    CommandHandler_t  handler;
    const char       *help_text;
} CLI_Command_t;

static int s_test_executed_cmd = 0;

static void Cmd_Help(int argc, char *argv[]);
static void Cmd_Led(int argc, char *argv[]);
static void Cmd_Status(int argc, char *argv[]);

static const CLI_Command_t s_commands[] = {
    { "help",   Cmd_Help,   "Display available commands" },
    { "led",    Cmd_Led,    "Control user LED: led <on|off>" },
    { "status", Cmd_Status, "Print system status and uptime" },
    { NULL,     NULL,       NULL }
};

static void Cmd_Help(int argc, char *argv[]) {
    (void)argc; (void)argv;
    s_test_executed_cmd = 1;
    printf("\r\n=== Available CLI Commands ===\r\n");
    for (size_t i = 0; s_commands[i].name != NULL; i++) {
        printf("  %-10s - %s\r\n", s_commands[i].name, s_commands[i].help_text);
    }
}

static void Cmd_Led(int argc, char *argv[]) {
    s_test_executed_cmd = 2;
    if (argc < 2) {
        printf("Usage: led <on|off>\r\n");
        return;
    }
    if (strcmp(argv[1], "on") == 0) {
        printf("LED turned ON\r\n");
    } else if (strcmp(argv[1], "off") == 0) {
        printf("LED turned OFF\r\n");
    } else {
        printf("Unknown LED state: %s\r\n", argv[1]);
    }
}

static void Cmd_Status(int argc, char *argv[]) {
    (void)argc; (void)argv;
    s_test_executed_cmd = 3;
    printf("Status: System Running, Heap Free: 8192 bytes\r\n");
}

void CLI_ProcessInput(char *line) {
    if (!line || strlen(line) == 0) return;

    char *argv[8];
    int argc = 0;

    char *token = strtok(line, " \t\r\n");
    while (token != NULL && argc < 8) {
        argv[argc++] = token;
        token = strtok(NULL, " \t\r\n");
    }

    if (argc == 0) return;

    for (size_t i = 0; s_commands[i].name != NULL; i++) {
        if (strcmp(argv[0], s_commands[i].name) == 0) {
            s_commands[i].handler(argc, argv);
            return;
        }
    }
    printf("Command '%s' not recognized. Type 'help'.\r\n", argv[0]);
}

int main(void) {
    printf("=== Running CLI Command Parser Tests ===\n");

    char test1[] = "help";
    CLI_ProcessInput(test1);
    assert(s_test_executed_cmd == 1);

    char test2[] = "led on";
    CLI_ProcessInput(test2);
    assert(s_test_executed_cmd == 2);

    char test3[] = "status";
    CLI_ProcessInput(test3);
    assert(s_test_executed_cmd == 3);

    printf("CLI Command Parser Tests PASSED successfully!\n");
    return 0;
}
