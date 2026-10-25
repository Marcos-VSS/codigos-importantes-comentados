### redshift

**redshift -O y**: quanto maior o número "y", maior a temperatura da cor do monitor. Ex.: y = 4000
**nohup redshift -O 2500 &**: manter o comando rodando em segundo plano
**redshift -x**: resetar o redshift

### curl

**curl link**: faz um GET no link em que você colocar
**curl -X POST link   -H "Content-Type: application/json"   -d '{"coluna1": elemento1, "coluna2": elemento2, ...}'**: Faz um POST no link que você colocar.-H significa header (cabeçalho) e -d significa data (dados). -X (ou --request) serve para definir o método HTTP que você deseja usar na requisição

### Outros

**sudo fuser -k porta/tcp**: encerra as operações em qualquer porta da rede. Ex.: porta = 3000 