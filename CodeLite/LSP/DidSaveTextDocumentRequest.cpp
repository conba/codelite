#include "LSP/DidSaveTextDocumentRequest.h"

LSP::DidSaveTextDocumentRequest::DidSaveTextDocumentRequest(const wxFileName& filename, const wxString& fileContent)
{
    SetMethod("textDocument/didSave");
    m_params.reset(new DidSaveTextDocumentParams());
    m_params->As<DidSaveTextDocumentParams>()->SetTextDocument(TextDocumentIdentifier(filename.GetFullPath()));
    m_params->As<DidSaveTextDocumentParams>()->SetText(fileContent);
}

LSP::DidSaveTextDocumentRequest::~DidSaveTextDocumentRequest() {}
