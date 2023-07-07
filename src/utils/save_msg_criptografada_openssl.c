#include <stdio.h>
#include <sqlite3.h> // para compilar use flag -lsqlite3
#include <openssl/aes.h> // para compilar use a flag -lcrypto

// Chave de criptografia (128 bits)
unsigned char encryptionKey[] = "myEncryptionKey123";

int encryptData(const char *input, int inputLength, unsigned char *output) {
    AES_KEY aesKey;
    AES_set_encrypt_key(encryptionKey, 128, &aesKey);
    AES_encrypt((const unsigned char *)input, output, &aesKey);
    return AES_BLOCK_SIZE;
}

int insertMessage(const char *user, const char *message) {
    sqlite3 *db;
    char *errorMsg;

    // Abrir o banco de dados SQLite
    int rc = sqlite3_open("database.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Criar a tabela se ela não existir
    const char *createTableQuery = "CREATE TABLE IF NOT EXISTS messages (user TEXT, message BLOB);";
    rc = sqlite3_exec(db, createTableQuery, 0, 0, &errorMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao criar a tabela: %s\n", errorMsg);
        sqlite3_free(errorMsg);
        sqlite3_close(db);
        return 1;
    }

    // Criptografar os dados
    unsigned char encryptedMessage[AES_BLOCK_SIZE];
    int encryptedLength = encryptData(message, strlen(message), encryptedMessage);

    // Preparar a consulta SQL para inserção de dados
    const char *insertQuery = "INSERT INTO messages (user, message) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, insertQuery, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Bind dos parâmetros da consulta (usuário e mensagem criptografada)
    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 2, encryptedMessage, encryptedLength, SQLITE_STATIC);

    // Executar a consulta
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Erro ao executar a consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    // Finalizar e fechar o banco de dados
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}


// o famoso teste unitario
int main() {
    const char *user = "coyas";
    const char *message = "Hello, world!";

    int result = insertMessage(user, message);
    if (result == 0) {
        printf("Mensagem inserida com sucesso.\n");
    } else {
        printf("Erro ao inserir mensagem.\n");
    }

    return 0;
}
