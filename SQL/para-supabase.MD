**Ver quais tabelas estão com RLS ativado:**\
SELECT\
    &emsp;tablename,\
    &emsp;rowsecurity\
FROM pg_tables\
WHERE schemaname = 'public'\
ORDER BY tablename;

**Desativar RLS de uma tabela:**\
ALTER TABLE public.nome-tabela\
DISABLE ROW LEVEL SECURITY;

**Ativar RLS de uma tabela:**\
ALTER TABLE public.nome-tabela\
ENABLE ROW LEVEL SECURITY;