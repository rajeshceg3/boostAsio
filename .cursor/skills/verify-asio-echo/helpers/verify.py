#!/usr/bin/env python3
import argparse,json,os,pathlib,re,selectors,shutil,socket,subprocess,tempfile
root=pathlib.Path(__file__).resolve().parents[4];p=argparse.ArgumentParser();p.add_argument('mode',choices=['doctor','drive']);p.add_argument('--include',default=os.environ.get('BOOST_INCLUDE','/usr/include'));p.add_argument('--out',default='.verification-evidence/asio');a=p.parse_args();out=pathlib.Path(a.out).resolve();out.mkdir(parents=True,exist_ok=True);r={'mode':a.mode,'ok':False,'checks':[]};child=None
try:
 cxx=os.environ.get('CXX','g++');assert shutil.which(cxx) and shutil.which('make'),'Compiler/make missing';assert (pathlib.Path(a.include)/'boost/asio.hpp').is_file(),'Boostheaders missing';assert (root/'main.cpp').is_file(),'Wrong source';r['checks'].append({'doctor':'compiler/make/Boost/source found'})
 if a.mode=='drive':
  before=subprocess.run(['git','status','--porcelain'],cwd=root,capture_output=True,text=True,check=True).stdout
  with tempfile.TemporaryDirectory(prefix='asio-echo-') as td:
   cmd=['make','-B','-f',str(root/'Makefile'),'main','CXX='+cxx,'CXXFLAGS=-I'+str(pathlib.Path(a.include).resolve())+' -pthread','SRCS='+str(root/'main.cpp'),'HEADERS='];b=subprocess.run(cmd,cwd=td,capture_output=True,text=True,timeout=40);(out/'build.log').write_text(b.stdout+b.stderr);assert b.returncode==0,'Build failed'
   binary=str(pathlib.Path(td)/'main')
   for args in [['-1'],['65536'],['abc'],['1','2']]:
    q=subprocess.run([binary]+args,capture_output=True,text=True,timeout=5);assert q.returncode==1 and q.stderr.startswith('Server error:'),'Invalidport contract';r['checks'].append({'invalid_args':args,'exit':q.returncode})
   with socket.socket() as held:
    held.bind(('127.0.0.1',0));held.listen();q=subprocess.run([binary,str(held.getsockname()[1])],capture_output=True,text=True,timeout=5);assert q.returncode==1 and 'Server error:' in q.stderr,'Busy port must fail';r['checks'].append({'owned_busyport':'nonzero/diagnostic, no client sent to held socket'})
   child=subprocess.Popen([binary,'0'],stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True,errors='backslashreplace')
   sel=selectors.DefaultSelector();sel.register(child.stdout,selectors.EVENT_READ);assert sel.select(timeout=5),'No startup line';line=child.stdout.readline();sel.close();m=re.fullmatch(r'Server listening on port (\d+)\.\.\.\n',line);assert m,'Bad startup';port=int(m[1]);assert 0<port<65536;r['port']=port
   for payload in [b'hello',b'with spaces\n',b'\x00binary\xff']:
    assert child.poll() is None,'Owned server exited'
    with socket.create_connection(('127.0.0.1',port),timeout=3) as sock:
     sock.sendall(payload);data=b''
     while True:
      part=sock.recv(4096)
      if not part:break
      data+=part
     assert data==b'Echo: '+payload,'Echo bytes mismatch';r['checks'].append({'payload_hex':payload.hex(),'response_hex':data.hex()})
   with socket.create_connection(('127.0.0.1',port),timeout=3) as sock:sock.shutdown(socket.SHUT_WR);assert sock.recv(1)==b'','EOF handling'
   child.terminate();stdout,stderr=child.communicate(timeout=5);assert stderr=='','Unexpected server error';(out/'server.log').write_text(line+stdout+stderr);child=None
  after=subprocess.run(['git','status','--porcelain'],cwd=root,capture_output=True,text=True,check=True).stdout;assert before==after,'Source state changed';r['checks'].append({'cleanup':'ownedserverreaped/tempbuildremoved/sourceunchanged'})
 r['ok']=True
except Exception as e:r['failure']=str(e)
finally:
 if child and child.poll() is None:child.terminate();child.communicate(timeout=5)
(out/(a.mode+'.json')).write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r));raise SystemExit(0 if r['ok'] else 1)
