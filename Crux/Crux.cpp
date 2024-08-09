// Crux.cpp : Defines the entry point for the application.

#include "Crux.h"
#include "Scanner.h"

bool Crux::hadError = false;

int main(int argc, char* argv[])
{
	// TODO: Implement REPL mode
	if (argc != 2) {
		printf("Usage: crux program.crux");
		return 1;
	}

	std::string filename = argv[1];

	Crux::runFile(filename);

	if (Crux::hadError) return 1;

	return 0;
}

void Crux::runFile(const std::string& filename)
{
	std::ifstream file(filename);

	std::string fileContent;

	if (!file.is_open())
	{
		printf("Could not open file %s\n", filename.c_str());
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		fileContent += line + "\n";
	}

	file.close();

	run(fileContent);
}

void Crux::runPrompt()
{
}

void Crux::run(const std::string& source)
{
	Scanner scanner = Scanner(source);
	std::vector<Token> tokens = scanner.scanTokens();

	for (auto& token : tokens) {
		printf("%s\n", token.toString().c_str());
	}
}

void Crux::error(int line, const std::string& message)
{
	report(line, "", message);
}

void Crux::report(int line, const std::string& where, const std::string& message)
{
	printf("[line %d] Error %s: %s\n", line, where.c_str(), message.c_str());
	Crux::hadError = true;
}
