//package UI.java.src;

import java.sql.*;

public class CrudtoDB {
    public static void main(String[] args) {
        Connection connection = null;

        try {
            // Carrega o driver JDBC do SQLite
            Class.forName("org.sqlite.JDBC");

            String caminho_db = "jdbc:sqlite:../../../database.db";

            // Estabelece a conexão com o banco de dados
            connection = DriverManager.getConnection(caminho_db);

            // Lê as mensagens da tabela "messages"
            String selectMessages = "SELECT * FROM messages";
            Statement statement = connection.createStatement();
            ResultSet resultSet = statement.executeQuery(selectMessages);

            // Exibe as mensagens
            while (resultSet.next()) {
                String user = resultSet.getString("user");
                String message = resultSet.getString("message");
                System.out.println("===== User : Mensagem ====");
                System.out.println(user + " : " + message);
            }
        } catch (ClassNotFoundException | SQLException e) {
            e.printStackTrace();
        } finally {
            try {
                if (connection != null) {
                    connection.close();
                }
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }
}
