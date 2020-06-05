#ifndef ui_h
#define ui_h

class frameSend {
    public:
        virtual ~frameSend(){};
        virtual void send(const String &data){};
};

class frameSendAll: public frameSend {
    private:
        AsyncWebSocket *ws;
    public:
        frameSendAll(AsyncWebSocket *server){ ws = server; }
        ~frameSendAll() { ws = nullptr; }
        void send(const String &data){ if (data.length()) ws->textAll(data); };
};

class frameSendClient: public frameSend {
    private:
        AsyncWebSocketClient *cl;
    public:
        frameSendClient(AsyncWebSocketClient *client){ cl = client; }
        ~frameSendClient() { cl = nullptr; }
        void send(const String &data){ if (data.length()) cl->text(data); };
};

class frameSendHttp: public frameSend {
    private:
        AsyncWebServerRequest *req;
        AsyncResponseStream *stream;
    public:
        frameSendHttp(AsyncWebServerRequest *request){ req = request; }
        ~frameSendHttp() { delete stream; req = nullptr; }
        void send(const String &data){
            if (!data.length()) return;
            stream = req->beginResponseStream(FPSTR("application/json"));
            stream->addHeader(FPSTR("Cache-Control"), FPSTR("no-cache, no-store, must-revalidate"));
            stream->print(data);
            req->send(stream);
        };
};

#endif