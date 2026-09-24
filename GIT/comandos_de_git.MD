# Comandos Importantes de GIT #

- **git init:** para iniciar o git na pasta onde está
- **git status:** para ver os arquivos colocados no git
- **git add "nome do arquivo":** só adiciona um arquivo que você especificar
- **git add .:** adiciona todos os arquivos da pasta
- **git commit -m "nome do commit":** cria um commit (só funciona quando hjá alterações), que seria mais um ponto na linha do tempo do projeto
- **git config --global user.email "seu email":** para informar ao git o seu email, necessário para realizar commit
- **git config --global user.name "seu nome":** para informar ao git o seu nome, necessário para realizar commit
- **git remote add origin "link https do projeto":** vincula seu repositório Git local a um repositório remoto sem mandar nada ainda
- **git checkout -b minha-nova-branch:** se não existir essa branch, ele cria e muda pra ela, se ela existir, só muda pra ela, tudo localmente
- **git push --set-upstream origin nome-da-branch:** para enviar os arquivos do repósitório local para o remoto (cuidado, pois ele envia direto), e tem que fazer isso para cada branch ao enviar os arquivos
- **git push:** usar para enviar os arquivos depois de usar git push --set-upstream origin
- **git pull:** usar para receber os arquivos depois de usar git push --set-upstream origin
- **git pull origin nome-da-branch:** usar para receber os arquivos quando o git não sabe de onde você quer receber, informando o nome da branch que quer receber
- **git switch nome-da-branch:** apenas muda para a branch que você especificou, se ela existir
- **git clone link-do-repositório:** baixar um projeto inteiro dentro de um servidor, lembrando que o clone cria tudo do zero, enquanto o pull apenas atualiza o que já existe

# Comandos de configuração do GIT no Linux #

- **sudo apt install gh:** instala o pacote GitHub CLI
- **gh auth login:** abre uma interface para você logar sua conta do GitHub pelo navegador (como no Windows); selecione GitHub.com, HTTPS, y em "Authenticate Git with your GitHub credentials?" e Login with a web browser.
- **gh auth setup-git:** configura o Git para usar o GitHub CLI como o seu gerenciador de credenciais; dessa forma, ele faz os clones, pushs e tudo mais sem solicitar token
- **git clone --branch _nome da branch no git_ --single-branch _link do repositório_ _nome local da pasta clone_:** para conseguir clonar somente uma branch

# Comandos para carregar todas as branchs do repositório se você clonar usando single-branch #

- **git config remote.origin.fetch "+refs/heads/*:refs/remotes/origin/*"**: Altera a regra de mapeamento (chamada de refspec) do remote origin nas configurações do seu repositório para baixar e atualizar todas as branchs
- **git fetch --all**: Conecta-se a todos os repositórios remotos configurados (nesse caso, apenas o origin) e baixa o histórico, commits e referências mais recentes
- **git branch -a**: mostra todas as branchs que foram carregadas