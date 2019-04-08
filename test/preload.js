
//开启多窗口模式
wkeSetNavigationToNewWindowEnable(true);

//打开开发者工具
wkeShowDevtools('WebInspector/inspector.html');

//替换hao123为自定义网页内容
wkeOnLoadUrlBegin(function(url, job){
	console.log("wkeOnLoadUrlBegin:" + url);
	if (url == 'https://www.hao123.com/')
		wkeNetHookRequest(job);
		
	return false;
});
wkeOnNetHookRequest(function(url, job, buf, len){
	console.log('wkeOnNetHookRequest:' + url);
	wkeNetSetData(job, '<html><body><a href="https://www.baidu.com">Hello.I am coming from Hao123. But I am not Hao123</a></body></html>');
});

wkeOnWindowClosing(function(){
	if (confirm('是否关闭'))
		return true;
	return false;
});

/*wkeOnCreateView(function(navType, url, features){
	alert('new window.');
	return null;
});*/

wkeOnDocumentReady(function(frameId){
	console.log('wkeOnDocumentReady:' + window.location.href);
});

//打开主页
if (GlobalData("has open home page") == undefined) {//条件是防止死循环打开主页
	wkeLoadUrl("https://www.hao123.com");
	GlobalData("has open home page", true);
}