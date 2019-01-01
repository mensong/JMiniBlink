
//开启多窗口模式
wkeSetNavigationToNewWindowEnable(true);

wkeOnLoadUrlBegin(function(url, job){
	console.log(url);
	if (url == 'https://www.hao123.com/')
		wkeNetHookRequest(job);
	
	return false;
});

wkeOnNetHookRequest(function(url, job, buf, len){
	//alert('hooked ' + url);
	alert(wkeNetGetMIMEType(job));
	wkeNetSetData(job, '<html><body><a href="https://www.baidu.com">Gergul</a></body></html>');
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
	alert('document ready: ' + window.location.href);
});