#include <lsp.hxx>
#include <server.hxx>

void handleInitialize(const json &request);
void handleShutDown(const json &request);

void runLSP()
{
    while (true)
    {
        std::string msg = readMessage();
        if (msg.empty())
            continue;
        json request = json::parse(msg, nullptr, false);
        if (request.is_discarded())
            continue;
        std::string method = request.value("method", "");
        if (method == "initialize")
        {
            handleInitialize(request);
        }
        else if (method == "textDocument/completion")
        {
            handleCompletion(request);
        }
        else if (method == "shutdown") {
            handleShutDown(request);
        }
        else if (method == "exit") {
            break;
        }
    }
}

void handleInitialize(const json &request) {
    json response;
    response["jsonrpc"] = "2.0";
    response["id"] = request["id"];
    response["result"] = {
        {"capabilities", {{"completionProvider", {{"resolveProvider", false}}}}}};
    sendMessage(response);
}

void handleShutDown(const json &request) {
    json response;
    response["jsonrpc"] = "2.0";
    response["id"] = request["id"];
    response["result"] = nullptr;
    sendMessage(response);
}