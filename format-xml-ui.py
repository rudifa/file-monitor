#! python

# This script formats an XML UI file by removing newlines and extra spaces within simple tags,
# to a format accepted by Qt.

import re


def format_ui_file(file_path):
    with open(file_path, 'r') as file:
        content = file.read()

    # Remove newlines and extra spaces within simple tags
    content = re.sub(
        r'<(\w+)>\s*\n?\s*([^<\n]+)\s*\n?\s*</\1>', r'<\1>\2</\1>', content)

    # Remove extra newlines
    content = re.sub(r'\n\s*\n', '\n', content)

    # Add minimal indentation (1 space)
    lines = content.split('\n')
    indent = 0
    formatted_lines = []
    open_tag_stack = []

    for line in lines:
        stripped_line = line.strip()

        # Check for closing tags at the start of the line
        if stripped_line.startswith('</') and open_tag_stack and open_tag_stack[-1] == stripped_line[2:-1]:
            indent = max(0, indent - 1)
            open_tag_stack.pop()

        # Add indentation
        formatted_lines.append(' ' * indent + stripped_line)

        # Check for opening and closing tags within the line
        tags = re.findall(r'</?(\w+)[^>]*>', stripped_line)
        for i, tag in enumerate(tags):
            if i == 0 and stripped_line.startswith('<' + tag):
                if not stripped_line.endswith('/>') and not '</' + tag + '>' in stripped_line:
                    indent += 1
                    open_tag_stack.append(tag)
            elif '</' + tag + '>' in stripped_line:
                if open_tag_stack and open_tag_stack[-1] == tag:
                    indent = max(0, indent - 1)
                    open_tag_stack.pop()

    formatted_content = '\n'.join(formatted_lines)

    with open(file_path, 'w') as file:
        file.write(formatted_content)


if __name__ == "__main__":
    format_ui_file('source/mainwindow.ui')
