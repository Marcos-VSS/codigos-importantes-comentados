## Noções de Segurança e Idempotência:
- Seguro: não altera o estado das coisas no servidor
- Idempotente: fazer a mesma requisição múltiplas vezes produz o mesmo estado no servidor

## GET
- Recupera dados de um recurso do servidor
- Seguro e Idempotente
- Exemplo:\
\
GET /usuarios/1 HTTP/1.1 *MÉTODO CAMINHO (URI/endpoint) VERSÃO DO HTTP\
Host: api.exemplo.com *URL base

## POST
- Envia dados para o servidor criar um novo recurso
- Não Seguro e não Idempotente
- Exemplo:\
\
POST /usuarios HTTP/1.1\
Host: api.exemplo.com\ 
Content-Type: application/json \
{\
  "nome": "Marcos",\
  "email": "marcos@email.com"\
}\
\
*Os dados são enviados no corpo (body) da requisição, geralmente em formato JSON
*Content-Type: application/json é um aviso prévio para o servidor de que O texto abaixo é estruturado no formato JSON

## PUT
- Atualiza **integralmente** um recurso existente ou o cria caso não exista no endereço especificado
- Não Seguro e Idempotente
- Exemplo:\
\
PUT /usuarios/1 HTTP/1.1\
Host: api.exemplo.com\
Content-Type: application/json\
{\
  "nome": "Marcos Silva",\
  "email": "marcos.silva@email.com"\
}\
\
*Não enviar um objeto com todas as informações completas pode resultar em problema na requisição ou mesmo na integralidade de certos dados do servidor (removendo ou deixando o que não devia). Ou seja, deve trabalhar com todos os campos obrigatórios

## PATCH
- Atualiza **parcialmente** um recurso existente
- Não Seguro e em certos casos Idempotente e em outros não
- Exemplo:\
\
PATCH /usuarios/1 HTTP/1.1\
Host: api.exemplo.com\
Content-Type: application/json\
{\
  "email": "novo_email@email.com"\
}\
\
*Deve trabalhar com apenas o(s) campo(s) que mudou(aram)

## DELETE
- Apaga um recurso específico
- Não Seguro e Idempotente (apagar a mesma coisa várias vezes não altera o estado do servidor)
- Exemplo:\
\
DELETE /usuarios/1 HTTP/1.1\
Host: api.exemplo.com