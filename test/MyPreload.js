
wkeShowDevtools('WebInspector/inspector.html');

LoadPlugins("Plugin.MFCTest.dll");
LoadPlugins("Plugin.System.dll");

console.log(CallExtend("System", "writeText", "1.txt", "我爱你mensong"));
console.log(CallExtend("System", "readText", "1.txt"));

wkeOnLoadUrlBegin(function(url, job){	
	
	if (url == "https://www.ip138.com") {
		wkeNetHookRequest(job);//触发wkeOnNetHookRequest事件
		return false;
	}
	
	var newUrl = url.replace("https://www.hao123.com", "https://www.baidu.com");
	if (newUrl != url) {
		console.log("把" + url + "重定向到" + newUrl);
		wkeNetChangeRequestUrl(job, newUrl);
	}
	
	return false;
});
	
wkeOnNetHookRequest(function(url, job, data, dataLen){
	return "<html><body>mensong replace https://www.ip138.com</body></html>";
});


//html到vue模型的同步
function syncModel(item){
	const event = document.createEvent('HTMLEvents');
	event.initEvent('input', false, true);
	item.dispatchEvent(event);
}

function autoLogin(username, password){
	//自动输入用户名
	document.getElementsByTagName("input")[0].value = username;
	syncModel(document.getElementsByTagName("input")[0]);
	//自动输入密码
	document.getElementsByTagName("input")[1].value = password;
	syncModel(document.getElementsByTagName("input")[1]);	

	//模拟点击
	var clickEvent = new MouseEvent('click');
	document.getElementsByClassName("btn")[0].dispatchEvent(clickEvent);// 派发
}


wkeOnDocumentReady(function(frameId){
	var url = wkeGetFrameUrl(frameId);
	console.log(url);
	wkeSetWindowTitle(wkeGetTitle());
	
	if (url == "https://www.baidu.com/"){
		var keyword = CallExtend("MFCTest", "foo1", 1,2,3,4);
		console.log(keyword);
		document.getElementById("kw").value = keyword;
	}
});


if (GlobalData("home page") == undefined) {
	wkeLoadUrl("https://www.baidu.com/");
	GlobalData("home page", true);
	CallExtend("System", "setIcon", "D:\\QT_with_debugstring\\src\\3rdparty\\webkit\\Source\\WebKit\\qt\\tests\\MIMESniffing\\resources\\image_vnd.microsoft.icon");
}
