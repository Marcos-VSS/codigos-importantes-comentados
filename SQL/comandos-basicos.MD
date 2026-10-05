**Criar novo banco de dados:**\
CREATE DATABASE nome-qualquer;

**Criar tabela para o banco de de dados:**\
CREATE TABLE nome-tabela( \
    &emsp;coluna1 tipo param, \
    &emsp;coluna2 tipo param \
)

**Adicionar registros na tabela:**\
INSERT INTO nome-tabela (coluna1, coluna2, ...) \
VALUES (val1, val2, ...), \
       &emsp;&emsp;&emsp;&nbsp;&nbsp;&nbsp;(val1, val2, ...) \
;

**Buscar os registros feitos na tabela:**
- **buscar todos os registros:**\
SELECT * FROM nome-tabela;

- **buscar registros em uma coluna específica:**\
SELECT nome-coluna FROM nome-tabela;

- **buscar um valor específico nos registros:**\
SELECT * FROM nome-tabela WHERE nome-coluna LIKE padrao;

- **buscar um valor completo nos registros:**\
SELECT * FROM nome-tabela WHERE nome-coluna = 'valor completo';

- **buscar valores específicos nos registros:**\
SELECT * FROM nome-tabela WHERE nome-coluna LIKE padrao1 OR nome-coluna LIKE padrao2;

<br>

**Ordenar os registros feitos na tabela:**
- **ordenar por uma coluna de a-z ou de 0-9 (cresente):**\
SELECT * FROM nome-tabela ORDER BY nome-coluna ASC;

- **ordenar por uma coluna de forma descrescente:**\
SELECT * FROM nome-tabela ORDER BY nome-coluna DESC;

<br>

**Limitar número de registros entregues:**\
SELECT * FROM nome-tabela LIMIT numero;

**Atualizar algum registro da tabela:**\
UPDATE nome-tabela SET nome-coluna = 'novo valor' WHERE elemento-unico = "escolhido";

**Deletar algum registro da tabela:**\
DELETE FROM usuarios WHERE elemento-unico = "escolhido";

**Adicionar colunas na tabela:**\
ALTER TABLE nome-tabela \
ADD COLUMN nova-coluna1 tipo padrao, \
ADD COLUMN nova-coluna2 tipo padrao \
;

**Excluir colunas de uma tabela:**\
ALTER TABLE nome-tabela \
DROP COLUMN nome-coluna1, \
DROP COLUMN nome-coluna2 \
;

**Excluir tabela:**\
DROP TABLE nome-tabela;

**Renomear uma coluna:**\
ALTER TABLE nome-tabela \
RENAME COLUMN nome-antigo TO nome-novo;

**Mudar o tipo de dado de um elemento e seus parâmetros:**\
ALTER TABLE nome-tabela \
  &emsp;**-- 2. Mudar o tipo de dado (com conversão explícita)**\
  &emsp;ALTER COLUMN nome_coluna TYPE NOVO_TIPO USING nome-coluna::NOVO_TIPO,\
  &emsp;**-- 3. (Opcional) Definir se aceita NULL ou NOT NULL**\
  &emsp;ALTER COLUMN nome-coluna SET NOT NULL,\
  &emsp;**-- 4. (Opcional) Definir ou remover o valor padrão (DEFAULT)**\
  &emsp;ALTER COLUMN nome-coluna SET DEFAULT 'valor_padrao';

**Colocar uma chave estrangeira pra uma coluna que está sendo criada referenciar:**\
CREATE TABLE nome-tabela (\
    &emsp;... \
\
    &emsp;CONSTRAINT fk_nome_qualquer **--sempre dê o nome para a restrição, fica mais fácil na manutenção**\
        &emsp;&emsp;FOREIGN KEY (nome-coluna)\
        &emsp;&emsp;REFERENCES nome-tabela-estrangeira(coluna-tabela-estrangeira)\
)

**Adicionar uma chave estrangeira pra uma coluna que já existe referenciar:**\
ALTER TABLE nome-tabela\
ADD CONSTRAINT fk_nome-qualquer\
FOREIGN KEY (nome-coluna)\
REFERENCES nome-tabela-estrangeira(coluna-tabela-estrangeira)\

**Resetar a sequência do tipo SERIAL para algum número base:**\
SELECT setval(pg_get_serial_sequence('nome-tabela', 'nome-coluna'), 1, false); **--O false é pra que o próximo item comece pelo número escolhido (nesse caso, foi escolhido 1)**\

<br>

__*#alguns parâmetros:*__
- PRIMARY KEY: a coluna servirá como chave primária
- NOT NULL: não aceita que registre uma nova linha sem colocar nada nesse elemento
- DEFAULT: o elemento dessa coluna recebe um valor automático se nenhum valor for passado na criação
- CURRENT_TIMESTAMP: retorna a data e a hora atuais do momento em que a instrução está sendo executada
- UNIQUE: não pode repetir um valor de outra linha referente à coluna em que foi colocado esse parãmetro

<br>

__*#em (val1, val2, ...), use '' só se os valores forem do tipo texto, data/hora ou caractere*__

__*#em padrao, você pode usar qualquer um desses exemplos:*__
- **'A%':** Encontra qualquer valor que comece com "A".
- **'%Silva'**: Encontra qualquer valor que termine com "Silva".
- **'%souza%'**: Encontra qualquer valor que contenha "souza" em qualquer parte do texto.
- **'Ca_o'**: Encontra "Caro", "Cabo", "Cato", mas não "Carro" (porque o sublinhado exige apenas uma letra no lugar dele)
- **'_a_a'**: Encontra palavras de 4 letras onde a segunda e a quarta letra são "a" (ex: "data", "faca").

<br>

__*#você também pode usar: SELECT * FROM nome-tabela ORDER BY nome-coluna ASC LIMIT numero; assim como junto à outros comandos de SELECT*__

__*#em elemento-unico é de bom tom usar a chave primaria*__

__*#se você quiser que a nova coluna adicionada seja NOT NULL, você deve adicionar um DEFAULT, que pode ser 0, por exemplo*__
